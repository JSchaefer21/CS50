SELECT DISTINCT name FROM people JOIN stars ON people.id=stars.person_id
WHERE movie_id IN
(SELECT movie_id FROM stars JOIN people ON people.id=stars.person_id
WHERE name LIKE 'Kevin Bacon' AND birth = 1958) AND name not like 'Kevin Bacon';