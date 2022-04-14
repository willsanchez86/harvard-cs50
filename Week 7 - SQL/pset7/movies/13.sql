-- In 13.sql, write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.
SELECT people.name
FROM people
INNER JOIN stars ON stars.person_id = people.id
INNER JOIN movies ON movies.id = stars.movie_id
WHERE movies.id IN (
    SELECT movies.id FROM movies
    INNER JOIN stars ON stars.movie_id = movies.id
    INNER JOIN people ON people.id = stars.person_id
    WHERE (people.name LIKE '%Kevin Bacon%' AND people.birth = 1958)
)
AND people.name NOT LIKE '%Kevin Bacon%';