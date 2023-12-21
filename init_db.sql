CREATE DATABASE task;
use task;

CREATE TABLE locations(hostname varchar(512) NOT NULL, latitude DECIMAL(14,12), longitude DECIMAL(15,12), PRIMARY KEY (hostname));

CREATE USER 'foo'@'localhost' IDENTIFIED BY 'bar';
GRANT INSERT, UPDATE, DELETE, SELECT  ON task.locations TO 'foo'@'localhost';

