-- Keep a log of any SQL queries you execute as you solve the mystery.

sqlite3 fiftyville.db
.schema
.tables
.schema crime_scene_reports

-->    CREATE TABLE crime_scene_reports (
-->    id INTEGER,
-->    year INTEGER,
-->    month INTEGER,
-->    day INTEGER,
-->    street TEXT,
-->    description TEXT,
-->    PRIMARY KEY(id)

.schema interviews

-->    CREATE TABLE interviews (
-->    id INTEGER,
-->    name TEXT,
-->    year INTEGER,
-->    month INTEGER,
-->    day INTEGER,
-->    transcript TEXT,
-->    PRIMARY KEY(id)

SELECT * FROM crime_scene_reports WHERE month = 7 AND day = 28 AND year = 2024
AND street = 'Humphrey Street';

-->Humphrey Street | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-->Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

-->297 | 2024 | 7     | 28  | Humphrey Street | Littering took place at 16:36. No known witnesses.

SELECT id, name, transcript FROM interviews
WHERE year = 2024 AND month = 7 AND day = 28;

-->| 158 | Jose    | “Ah,” said he, “I forgot that I had not seen you for some weeks. It is a little souvenir from the King of Bohemia in return for my assistance in the case of the Irene Adler papers.” (NOT USE FULL)
-->| 159 | Eugene  | “I suppose,” said Holmes, “that when Mr. Windibank came back from France he was very annoyed at your having gone to the ball.”
-->| 160 | Barbara | “You had my note?” he asked with a deep harsh voice and a strongly marked German accent. “I told you that I would call.” He looked from one to the other of us, as if uncertain which to address.
-->| 161 | Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.
-->| 162 | Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.
-->| 163 | Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.
-->| 191 | Lily    | Our neighboring courthouse has a very annoying rooster that crows loudly at 6am every day. My sons Robert and Patrick took the rooster to a city far, far away, so it may never bother us again. My sons have successfully arrived in Paris.

--> WE HAVE TO SEE WHAT CARS LEFT THE PRAKING = Emma's bakery at 10:15am

SELECT minute, activity, license_plate FROM bakery_security_logs
WHERE year = 2024 AND month = 7 AND day = 28
AND hour = 10 AND minute BETWEEN 15 and 25
ORDER BY minute;

--> +--------+----------+---------------+
--> | minute | activity | license_plate |
--> +--------+----------+---------------+
--> | 16     | exit     | 5P2BI95       |
--> | 18     | exit     | 94KL13X       |
--> | 18     | exit     | 6P58WS2       |
--> | 19     | exit     | 4328GD8       |
--> | 20     | exit     | G412CB7       |
--> | 21     | exit     | L93JTIZ       |
--> | 23     | exit     | 322W7JE       |
--> | 23     | exit     | 0NTHK55       |
--> +--------+----------+---------------+

--> Now we have all the plates that left the parking of Emmas bakery between 10:15 and 10:25

SELECT name, phone_number, passport_number, license_plate
FROM people
WHERE license_plate
IN('5P2BI95', '94KL13X', '6P58WS2', '4328GD8', 'G412CB7', 'L93JTIZ', '322W7JE', '0NTHK55');

--> AND WE GET THIS RESULT
--> +---------+----------------+-----------------+---------------+
--> |  name   |  phone_number  | passport_number | license_plate |
--> +---------+----------------+-----------------+---------------+
--> | Vanessa | (725) 555-4692 | 2963008352      | 5P2BI95       |
--> | Barry   | (301) 555-4174 | 7526138472      | 6P58WS2       |
--> | Iman    | (829) 555-5269 | 7049073643      | L93JTIZ       |
--> | Sofia   | (130) 555-0289 | 1695452385      | G412CB7       |
--> | Luca    | (389) 555-5198 | 8496433585      | 4328GD8       |
--> | Diana   | (770) 555-1861 | 3592750733      | 322W7JE       |
--> | Kelsey  | (499) 555-9472 | 8294398571      | 0NTHK55       |
--> | Bruce   | (367) 555-5533 | 5773159633      | 94KL13X       |
--> +---------+----------------+-----------------+---------------+

--> Now that we have the people that left the parking lot at that hour,
--> we can check the people that withdrawed money form the ATM as,
--> Eugene saw the thief withdraw money.

SELECT DISTINCT people.name FROM people
JOIN bank_accounts ba ON ba.person_id = people.id
JOIN atm_transactions atm ON atm.account_number = ba.account_number
WHERE atm.year = 2024 AND atm.month = 7 AND atm.day = 28
AND atm.transaction_type = 'withdraw'
AND atm.atm_location = 'Leggett Street';

--> And we get the people that withdrawed money from the atm
--> +---------+
--> |  name   |
--> +---------+
--> | Luca    |
--> | Kenny   |
--> | Taylor  |
--> | Bruce   |
--> | Brooke  |
--> | Iman    |
--> | Benista |
--> | Diana   |
--> +---------+

-->We have to compare the people that withdrawed money and left the parking

SELECT DISTINCT people.name, people.phone_number FROM people
JOIN phone_calls c ON c.caller = people.phone_number
WHERE c.year = 2024 AND c.month = 7 AND c.day = 28
AND c.duration < 60;

-->+---------+----------------+
-->|  name   |  phone_number  |
-->+---------+----------------+
-->| Sofia   | (130) 555-0289 |
-->| Kelsey  | (499) 555-9472 |
-->| Bruce   | (367) 555-5533 |
-->| Taylor  | (286) 555-6063 |
-->| Diana   | (770) 555-1861 |
-->| Carina  | (031) 555-6622 |
-->| Kenny   | (826) 555-1652 |
-->| Benista | (338) 555-6650 |
-->+---------+----------------+

--> Now we search for the earliest flight out of Fiftyville the (29/07/2024)

SELECT id FROM airports WHERE city = 'Fiftyville';

--> We get the ID = 8 For the airport

SELECT id, destination_airport_id, hour, minute FROM flights
WHERE year = 2024 AND month = 7 AND day = 29 AND origin_airport_id = 8
ORDER BY hour, minute
LIMIT 5;

-->+----+------------------------+------+--------+
-->| id | destination_airport_id | hour | minute |
-->+----+------------------------+------+--------+
-->| 36 | 4                      | 8    | 20     |
-->| 43 | 1                      | 9    | 30     |
-->| 23 | 11                     | 12   | 15     |
-->| 53 | 9                      | 15   | 20     |
-->| 18 | 6                      | 16   | 0      |
-->+----+------------------------+------+--------+

--> AND we get the first flight -->| 36 | 4   8    | 20     |
--> Whe search for the ID 4 thats the destination and flight ID 36

SELECT city FROM airports WHERE id = 4;

--> And we get the destination that is New York City
--> Who went in this flight?

SELECT people.name FROM passengers pa
JOIN people ON people.passport_number = pa.passport_number
WHERE pa.flight_id = 36;

-->+--------+
-->|  name  |
-->+--------+
-->| Doris  |
-->| Sofia  |
-->| Bruce  |
-->| Edward |
-->| Kelsey |
-->| Taylor |
-->| Kenny  |
-->| Luca   |
-->+--------+

--> AND we get that Bruce was the one on the flight
--> We search for phone reciver and duration

SELECT c.receiver, c.duration FROM phone_calls c
WHERE c.year = 2024 AND c.month = 7 AND c.day = 28
AND c.caller = (SELECT phone_number FROM people WHERE name ='Bruce')
AND c.duration < 60;

-->+----------------+----------+
-->|    receiver    | duration |
-->+----------------+----------+
-->| (375) 555-8161 | 45       |
-->+----------------+----------+

SELECT name FROM people WHERE phone_number = '(375) 555-8161';

-->+-------+
-->| name  |
-->+-------+
-->| Robin |
-->+-------+

-- So we have that Bruce is the thief and he flew to New York and his complice
-- is Robin
