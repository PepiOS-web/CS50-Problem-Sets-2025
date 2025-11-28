from cs50 import get_float

def main():
    # Ask for input until user provides a valid positive number
    while True:
        try:
            cents = get_float("Change owed: ")
            if cents > 0:
                break
        except ValueError:
            # If the input is not numeric, ask again
            continue

    # Convert dollars to cents and round to avoid floating-point errors
    cents = round(cents * 100)

    coins = 0

    # Calculate coins
    coins += cents // 25
    cents %= 25

    coins += cents // 10
    cents %= 10

    coins += cents // 5
    cents %= 5

    coins += cents

    print(coins)

# Run the program
if __name__ == "__main__":
    main()
