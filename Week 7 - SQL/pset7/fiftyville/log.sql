-- Keep a log of any SQL queries you execute as you solve the mystery.
-- theft took place on July 28, 2021  on Humphrey Street.

-- Gather Crime Scene report for that day on Humphrey Street
SELECT description FROM crime_scene_reports
WHERE (year = 2021 AND month = 7 AND day = 28 AND street LIKE '%Humphrey Street%');
-- -------------------------------------------------------------------------------------------------------------------------------------------------------+
-- | Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. |


-- Gather interviews on and after July that have discussed the bakery specifically
SELECT month,day,name,transcript FROM interviews
WHERE (year = 2021 AND month >= 7)
AND transcript LIKE '%bakery%'
ORDER BY month ASC, day ASC;
-- Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
-- Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
-- Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
-- Emma    | I'm the bakery owner, and someone came in, suspiciously whispering into a phone for about half an hour. They never bought anything.


-- Run several combined subqueries using the clues from interviews to find possible matches. Include the
SELECT people.name, people.passport_number, people.license_plate, people.phone_number,bank_accounts.account_number, phone_calls.receiver
FROM people
    INNER JOIN bank_accounts
    ON bank_accounts.person_id = people.id
    INNER JOIN phone_calls
    ON phone_calls.caller = people.phone_number
        AND phone_calls.year = 2021
        AND phone_calls.month = 7
        AND phone_calls.day = 28
        AND phone_calls.duration < 60
-- Check if person's license plate was spotted exiting the parking garage within 10 minutes of theft
WHERE people.license_plate IN
    (SELECT license_plate
    FROM bakery_security_logs
    WHERE (year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND activity LIKE '%exit%' AND minute BETWEEN 14 AND 26))
-- Check if person's bank account matches atm withdrawals on Leggett street that day
AND bank_accounts.account_number IN
    (SELECT account_number
    FROM atm_transactions
    WHERE (year = 2021 AND month = 7 AND day = 28 AND transaction_type = 'withdraw' AND atm_location = 'Leggett Street'))
-- Check if person made a phone call less than one minute long that day
AND people.phone_number IN
    (SELECT caller
    FROM phone_calls
    WHERE (year = 2021 AND month = 7 AND day = 28 AND duration < 60));
-- +-------+-----------------+---------------+----------------+----------------+----------------+
-- | name  | passport_number | license_plate |  phone_number  | account_number |    receiver    |
-- +-------+-----------------+---------------+----------------+----------------+----------------+
-- | Bruce | 5773159633      | 94KL13X       | (367) 555-5533 | 49610011       | (375) 555-8161 |
-- | Diana | 3592750733      | 322W7JE       | (770) 555-1861 | 26013199       | (725) 555-3243 |
-- +-------+-----------------+---------------+----------------+----------------+----------------+

-- Get names of the receivers from Bruce or Diane's phone call
SELECT name, passport_number, phone_number
FROM people
WHERE (phone_number = '(375) 555-8161' OR phone_number = '(725) 555-3243');

-- +--------+-----------------+----------------+
-- |  name  | passport_number |  phone_number  |
-- +--------+-----------------+----------------+
-- | Philip | 3391710505      | (725) 555-3243 |
-- | Robin  |                 | (375) 555-8161 |
-- +--------+-----------------+----------------+


-- Find the matching passport ID of the person who takes the first flight out of Fiftyville the following daycat
SELECT people.name, flights.id, passengers.passport_number, passengers.seat, flights.destination_airport_id, flights.hour, flights.minute
FROM flights
    INNER JOIN airports
    ON airports.id = flights.origin_airport_id
        AND airports.city LIKE '%Fiftyville%'
    INNER JOIN passengers
    ON passengers.flight_id = flights.id
    INNER JOIN people
    ON people.passport_number = passengers.passport_number
WHERE passengers.passport_number IN (3592750733, 3391710505, 5773159633)
AND flights.month = 7
AND flights.day = 29;
-- +-------+----+-----------------+------+------------------------+------+--------+
-- | name  | id | passport_number | seat | destination_airport_id | hour | minute |
-- +-------+----+-----------------+------+------------------------+------+--------+
-- | Diana | 18 | 3592750733      | 4C   | 6                      | 16   | 0      |
-- | Bruce | 36 | 5773159633      | 4A   | 4                      | 8    | 20     |
-- +-------+----+-----------------+------+------------------------+------+--------+

-- Find the destination airport for flight ID 36
SELECT airports.city
FROM airports
    INNER JOIN flights
    ON airports.id = flights.destination_airport_id
        AND flights.id = 36;