SELECT name, duration FROM Track ORDER BY duration DESC LIMIT 1;
SELECT name FROM Track WHERE duration >= 3.5;
SELECT name FROM Collection WHERE release_year BETWEEN 2018 AND 2020;
SELECT name FROM Artist WHERE name NOT LIKE '% %';
SELECT name FROM Track WHERE LOWER(name) LIKE '%мой%' OR LOWER(name) LIKE '%my%';