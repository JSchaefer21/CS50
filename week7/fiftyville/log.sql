-- Keep a log of any SQL queries you execute as you solve the mystery.

-- All you know is that the theft took place on July 28, 2021 and that it took place on Humphrey Street.


--I have cheked the report from the day, month, year and street we already know, and i get the following information
SELECT description FROM crime_scene_reports WHERE street like 'Humphrey Street' AND day=28 AND month=7 AND year=2021;
--hour: 10:15am
--three witnesses at 10:15
--bakery


--then we chek the transcription from the interviews that took place the same day and get the following:
SELECT transcript FROM interviews WHERE day=28 AND month=7 AND year=2021;
--thief take a car in the bakery parking with in 10 minuts
--thief retires money from the bank at morning at Leggett Street
--thief make a call <1min
--earlist flight day 29
--2 thief purchase the ticked


--lets see first the plates and time from the bakery park that day 28, order by time
SELECT hour, minute, license_plate, activity FROM bakery_security_logs
WHERE day=28 AND month=7 AND year=2021 AND hour=10 ORDER BY hour, minute;
--and now lets see the entry hour filtering with the plates from the possible exit hour
SELECT hour, minute, license_plate, activity FROM bakery_security_logs WHERE activity="entrance" AND license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day=28 AND month=7 AND year=2021 AND hour=10
AND minute >=15 AND minute<=25 AND activity="exit") ORDER BY hour, minute;
--the thief entry between 8:18 - 9:28
--5 possible plates


--lets see then the activity in the bank between 8:18 and 9:28 (there is no time :( in the table )
SELECT atm_location, amount, transaction_type FROM atm_transactions WHERE year=2021 AND month=7 AND day=28
AND atm_location="Leggett Street" AND transaction_type="withdraw";
--this information will be usefull later, together with the bank acount name


--time to chek the calls
SELECT caller, receiver, duration FROM phone_calls WHERE year=2021 AND month=7 AND day=28 AND duration <60;
--
SELECT name FROM people WHERE phone_number IN
(SELECT caller FROM phone_calls WHERE year=2021 AND month=7 AND day=28 AND duration <60);
--not very usefull information


--now we can check the names according to the possible phone number and plates we get in the previous analisys (later will see also with the passport)
SELECT name FROM people
WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year=2021 AND month=7 AND day=28 AND duration <60)
AND license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE activity="entrance" AND license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day=28 AND month=7 AND year=2021 AND hour=10
AND minute >=15 AND minute<=25 AND activity="exit"));
-- time-marge from 10:15 to 10:25
-- we have 4 possible persons!!


--im gonna create a table with the 5 possible persors and their passport, in order to work easyly
CREATE TABLE thief (name TEXT, passport TEXT);
INSERT INTO thief SELECT name, passport_number FROM people
WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year=2021 AND month=7 AND day=28 AND duration <60)
AND license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE activity="entrance" AND license_plate IN
(SELECT license_plate FROM bakery_security_logs WHERE day=28 AND month=7 AND year=2021 AND hour=10
AND minute >15 AND minute<25 AND activity="exit"));


--time to chek the information about the fligh and airport
-- day 29, ticked buyed for another persor
SELECT * FROM airports;
SELECT * FROM flights WHERE year=2021 AND month=7 AND day=29;


-- lets' see who take the flight in day 29 and also is possible thief
SELECT name FROM thief WHERE passport IN
(SELECT passport_number FROM passengers WHERE flight_id IN
(SELECT id FROM flights WHERE year=2021 AND month=7 AND day=29 AND origin_airport_id=8));
--still have 4 suspictios


--now we will cheked together with the bank accounts
SELECT * FROM thief WHERE name IN
(SELECT name FROM people WHERE id IN
(SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE year=2021 AND month=7 AND day=28
AND atm_location="Leggett Street" AND transaction_type="withdraw")));
--and we have 2 possible persons that:
--exit from parking between 10:15 - 10:25
--entry ar morning between the same day
--make a call <1min the same day
--make a withdraw the same day in Leggett Street
--take a flight the day before

-- i JUST have TWO possible person: DIANA or BRUCE
-- lets see at what time they take the flight
SELECT name, hour FROM thief JOIN passengers ON passengers.passport_number=thief.passport JOIN flights ON passengers.flight_id=flights.id
WHERE year=2021 AND month=7 AND day=29 AND origin_airport_id=8;
-- DIana takes a very late flight and bruce very erarlies, so we can confirm that is was BRUCE!!!


--lets see where he goes
SELECT * FROM airports WHERE id IN
(SELECT destination_airport_id FROM flights WHERE year=2021 AND month=7 AND day=29 AND id IN
(SELECT flight_id FROM passengers WHERE passport_number IN
(SELECT passport FROM thief WHERE name ="Bruce")));
--she goes to NEW YORK!!!


--finally we need the accomplice
SELECT name FROM people WHERE phone_number IN
(SELECT receiver FROM phone_calls WHERE year=2021 AND month=7 AND day=28 AND duration <60 AND caller IN
(SELECT phone_number FROM people WHERE name="Bruce"));
-- the accomplice is ROBIN!!!!!