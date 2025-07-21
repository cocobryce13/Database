CREATE TABLE Genre (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL
);

CREATE TABLE Artist (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL
);

CREATE TABLE ArtistGenre (
    genre INTEGER NOT NULL REFERENCES Genre(id),
    artist INTEGER NOT NULL REFERENCES Artist(id),
    PRIMARY KEY (genre, artist)
);

CREATE TABLE Album (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    release_year INTEGER NOT NULL
);

CREATE TABLE ArtistAlbum (
    artist INTEGER NOT NULL REFERENCES Artist(id),
    album INTEGER NOT NULL REFERENCES Album(id),
    PRIMARY KEY (artist, album)
);

CREATE TABLE Track (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    duration DECIMAL(5,2) NOT NULL,
    album INTEGER NOT NULL REFERENCES Album(id)
);

CREATE TABLE Collection (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    release_year INTEGER NOT NULL
);

CREATE TABLE CollectionTrack (
    collection INTEGER NOT NULL REFERENCES Collection(id),
    track INTEGER NOT NULL REFERENCES Track(id),
    PRIMARY KEY (collection, track)
);