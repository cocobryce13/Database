-- ЗАДАНИЕ 3: SELECT-ЗАПРОСЫ

SELECT g.name, COUNT(ag.artist) AS artist_count FROM Genre g
JOIN ArtistGenre ag ON g.id = ag.genre GROUP BY g.name;

SELECT COUNT(t.id) AS track_count FROM Track t
JOIN Album a ON t.album = a.id WHERE a.release_year BETWEEN 2019 AND 2020;

SELECT a.name, AVG(t.duration) AS avg_duration FROM Album a
JOIN Track t ON a.id = t.album GROUP BY a.name;

SELECT DISTINCT ar.name FROM Artist ar WHERE ar.id NOT IN (
    SELECT aa.artist FROM ArtistAlbum aa
    JOIN Album al ON aa.album = al.id WHERE al.release_year = 2020
);

SELECT DISTINCT c.name FROM Collection c
JOIN CollectionTrack ct ON c.id = ct.collection
JOIN Track t ON ct.track = t.id
JOIN Album a ON t.album = a.id
JOIN ArtistAlbum aa ON a.id = aa.album
JOIN Artist ar ON aa.artist = ar.id
WHERE ar.name = 'Muse';