DROP TABLE IF EXISTS PRESENTATION CASCADE;
DROP TABLE IF EXISTS ROOM CASCADE;
DROP TABLE IF EXISTS TICKET CASCADE ;
DROP TABLE IF EXISTS CONFERENCE CASCADE;
DROP TABLE IF EXISTS USER CASCADE;

CREATE TABLE IF NOT EXISTS USER (
    id_user INT AUTO_INCREMENT KEY,
    name VARCHAR(255),
    surname VARCHAR(255),
    email varchar(255),
    username VARCHAR(255),
    password VARBINARY(128),
    role  VARCHAR(255)
)ENGINE = INNODB;

CREATE TABLE IF NOT EXISTS CONFERENCE(
    id_conference INT AUTO_INCREMENT KEY,
    field_of_interest VARCHAR(255),
    name VARCHAR(255),
    place varchar(255),
    price FLOAT,
    date_of_start DATETIME,
    date_of_end DATETIME,
    capacity INT NOT NULL,
    user_fk INT,
    FOREIGN KEY (user_fk) REFERENCES USER(id_user) on delete cascade on update cascade
)ENGINE = INNODB;

CREATE TABLE IF NOT EXISTS TICKET(
    id_ticket INT AUTO_INCREMENT KEY,
    conference_fk INT,
    user_fk INT,
    status TEXT,
    name VARCHAR(255),
    surname varchar(255),
    FOREIGN KEY (conference_fk) REFERENCES CONFERENCE(id_conference) on delete cascade on update cascade,
    FOREIGN KEY (user_fk) REFERENCES USER(id_user) ON DELETE SET NULL on update cascade
)ENGINE = INNODB;

CREATE TABLE  ROOM(
    id_room INT AUTO_INCREMENT KEY,
    capacity INT NOT NULL,
    name VARCHAR(50),
    equipment TEXT,
    conference_fk INT,
    FOREIGN KEY (conference_fk) REFERENCES CONFERENCE(id_conference) on delete cascade
)ENGINE = INNODB;

CREATE TABLE IF NOT EXISTS PRESENTATION(
  id_prezentation INT AUTO_INCREMENT KEY,
  name VARCHAR(255),
  info VARCHAR(255),
  lecturer VARCHAR(255),
  tags VARCHAR(255),
  date_of_start  DATETIME,
  date_of_end DATETIME,
  status varchar(255),
  user_fk INT,
  room_fk INT,
  conference_fk INT,
  FOREIGN KEY (room_fk) references ROOM(id_room) ON DELETE SET NULL on update cascade,
  FOREIGN KEY (conference_fk) references  CONFERENCE(id_conference)on delete cascade on update cascade
)ENGINE = INNODB;

INSERT INTO USER(id_user, name, surname, email, username, password, role)  VALUES(0,'Admin','Admin','admin@admin.sk','admin',0x44B236956A4DAF24,'admin');

ALTER TABLE user CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
ALTER TABLE room CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
ALTER TABLE presentation CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
ALTER TABLE ticket CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
ALTER TABLE conference CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
