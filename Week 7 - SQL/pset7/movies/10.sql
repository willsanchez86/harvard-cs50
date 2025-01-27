-- In 10.sql, write a SQL query to list the names of all people who have directed a movie that received a rating of at least 9.0.
SELECT DISTINCT people.name
FROM people
INNER JOIN directors ON directors.person_id = people.id
INNER JOIN ratings ON ratings.movie_id = directors.movie_id
WHERE (ratings.rating >= 9.0);