-- ЗАДАНИЕ 1: СОЗДАНИЕ И ЗАПОЛНЕНИЕ ТАБЛИЦ

CREATE TABLE Genre (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL
);

CREATE TABLE Artist (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL
);

CREATE TABLE ArtistGenre (
    genre INTEGER REFERENCES Genre(id),
    artist INTEGER REFERENCES Artist(id),
    PRIMARY KEY (genre, artist)
);

CREATE TABLE Album (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    release_year INTEGER NOT NULL
);

CREATE TABLE ArtistAlbum (
    artist INTEGER REFERENCES Artist(id),
    album INTEGER REFERENCES Album(id),
    PRIMARY KEY (artist, album)
);

CREATE TABLE Track (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    duration DECIMAL(5,2) NOT NULL,
    album INTEGER REFERENCES Album(id)
);

CREATE TABLE Collection (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    release_year INTEGER NOT NULL
);

CREATE TABLE CollectionTrack (
    collection INTEGER REFERENCES Collection(id),
    track INTEGER REFERENCES Track(id),
    PRIMARY KEY (collection, track)
);

-- Заполнение таблиц
INSERT INTO Genre (name) VALUES ('Rock'), ('Pop'), ('Metal');
INSERT INTO Artist (name) VALUES ('Muse'), ('Adele'), ('Rammstein'), ('Ария');
INSERT INTO ArtistGenre (artist, genre) VALUES (1, 1), (2, 2), (3, 3), (4, 1), (4, 3);
INSERT INTO Album (name, release_year) VALUES ('Simulation Theory', 2018), ('25', 2015), ('Liebe ist für alle da', 2009);
INSERT INTO ArtistAlbum (artist, album) VALUES (1, 1), (2, 2), (3, 3), (4, 3);
INSERT INTO Track (name, duration, album) VALUES
  ('Algorithm', 4.50, 1), ('The Dark Side', 3.45, 1),
  ('Hello', 4.55, 2), ('Send My Love', 3.43, 2),
  ('Pussy', 3.59, 3), ('My Land', 2.30, 3);
INSERT INTO Collection (name, release_year) VALUES ('Лучшее 2018', 2018), ('Топ 2020', 2020), ('Rock Vibes', 2019), ('The Best of Pop', 2018);
INSERT INTO CollectionTrack (collection, track) VALUES (1, 1), (1, 3), (2, 2), (2, 4), (3, 5), (4, 6);