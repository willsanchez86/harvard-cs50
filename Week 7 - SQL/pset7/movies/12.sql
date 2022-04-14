-- In 12.sql, write a SQL query to list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred.
SELECT DISTINCT movies.title
FROM movies
INNER JOIN stars ON stars.movie_id = movies.id
INNER JOIN people ON people.id = stars.person_id
WHERE people.name LIKE '%Johnny Depp%'
AND title IN (
    SELECT movies.title FROM movies
    INNER JOIN STARS ON stars.movie_id = movies.id
    INNER JOIN people ON people.id = stars.person_id
    WHERE people.name LIKE '%Helena Bonham Carter%'
);