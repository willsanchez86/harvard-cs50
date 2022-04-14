-- In 7.sql, write a SQL query that returns the average energy of songs that are by Drake.
SELECT AVG(energy)
FROM songs
JOIN artists
on songs.artist_id = artists.id
WHERE (artists.name LIKE '%Drake%');