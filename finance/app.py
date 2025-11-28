import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    rows = db.execute(
        """
        SELECT symbol, SUM(shares) AS total_shares
        FROM transactions
        WHERE user_id = ?
        GROUP BY symbol
        HAVING total_shares > 0
        """,
        session["user_id"]
    )

    holdings = []
    grand_total = 0

    # 2) For each symbol, find the price and calculate the value.
    for row in rows:
        symbol = row["symbol"]
        shares = row["total_shares"]

        quote = lookup(symbol)
        price = quote["price"]
        total = price * shares

        grand_total += total

        holdings.append({
            "symbol": symbol,
            "shares": shares,
            "price": price,
            "total": total
        })

    # 3) Check user cash
    cash_row = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = cash_row[0]["cash"]

    grand_total += cash

    # 4) Render template
    return render_template("index.html", holdings=holdings, cash=cash, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # 1) Read symbol and shares
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # 2) Validate symbol
        if not symbol:
            return apology("must provide symbol")

        # 3) Validate shares
        if not shares:
            return apology("must provide shares")

        # 4) Check that shares is a positive integer
        if not shares.isdigit() or int(shares) <= 0:
            return apology("shares must be a positive integer")

        shares = int(shares)

        # 5) Get quote
        quote = lookup(symbol)
        if quote is None:
            return apology("invalid symbol")

        price = quote["price"]
        total_cost = price * shares

        # 6) Check user's cash
        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = rows[0]["cash"]

        # 7) Check if they can afford it
        if cash < total_cost:
            return apology("can't afford")

        # 8) Insert transaction (PURCHASE = positive shares)
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
            session["user_id"], symbol.upper(), shares, price
        )

        # 9) Update user's cash
        db.execute(
            "UPDATE users SET cash = cash - ? WHERE id = ?",
            total_cost, session["user_id"]
        )

        # 10) Return to index
        return redirect("/")

    else:
        # GET: purchase form
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute(
        """
        SELECT symbol, shares, price, transacted
        FROM transactions
        WHERE user_id = ?
        ORDER BY transacted DESC
        """,
        session["user_id"]
    )

    # 2) Render template
    return render_template("history.html", transactions=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""
    if request.method == "POST":
        if not request.form.get("username"):  # Check for empty username
            return apology("must provide username", 403)
        elif request.form.get("password") == "":  # Check for empty password
            return apology("must provide password", 403)

        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        session["user_id"] = rows[0]["id"]
        return redirect("/")
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("must provide symbol")

        quote = lookup(symbol)

        if quote is None:
            return apology("invalid symbol")

        return render_template("quoted.html", quote=quote)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        if not username:
            return apology("must provide username")

        if not password:
            return apology("must provide password")

        if not confirmation:
            return apology("must confirm password")

        if password != confirmation:
            return apology("passwords do not match")

        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(rows) != 0:
            return apology("username already exists")

        hash_pw = generate_password_hash(password)

        db.execute(
            "INSERT INTO users (username, hash) VALUES (?, ?)",
            username,
            hash_pw,
        )

        new_id = db.execute(
            "SELECT id FROM users WHERE username = ?",
            username,
        )[0]["id"]

        session["user_id"] = new_id

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("must provide symbol")

        if not shares:
            return apology("must provide shares")

        if not shares.isdigit() or int(shares) <= 0:
            return apology("shares must be a positive integer")

        shares = int(shares)

        rows = db.execute(
            """
            SELECT SUM(shares) AS total_shares
            FROM transactions
            WHERE user_id = ? AND symbol = ?
            """,
            session["user_id"], symbol
        )

        owned = rows[0]["total_shares"]
        if owned is None:
            owned = 0

        if shares > owned:
            return apology("too many shares")

        quote = lookup(symbol)
        if quote is None:
            return apology("invalid symbol")

        price = quote["price"]
        total_value = price * shares

        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price) VALUES (?, ?, ?, ?)",
            session["user_id"], symbol.upper(), -shares, price
        )

        db.execute(
            "UPDATE users SET cash = cash + ? WHERE id = ?",
            total_value, session["user_id"]
        )

        return redirect("/")

    else:
        rows = db.execute(
            """
            SELECT symbol, SUM(shares) AS total_shares
            FROM transactions
            WHERE user_id = ?
            GROUP BY symbol
            HAVING total_shares > 0
            """,
            session["user_id"]
        )

        return render_template("sell.html", symbols=rows)
