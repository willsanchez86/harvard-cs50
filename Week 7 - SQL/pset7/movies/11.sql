-- In 11.sql, write a SQL query to list the titles of the five highest rated movies (in order) that Chadwick Boseman starred in, starting with the highest rated.
SELECT movies.title
FROM movies
INNER JOIN ratings ON ratings.movie_id = movies.id
INNER JOIN stars on stars.movie_id = movies.id
INNER JOIN people on people.id = stars.person_id
WHERE people.name LIKE '%Chadwick Boseman%'
ORDER BY ratings.rating DESC
LIMIT 5;