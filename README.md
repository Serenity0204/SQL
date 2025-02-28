# SQL

A relational database system built in C++, provided with a custom SQL-like language, all of them were built from ground up. This project involved creating essential data structures, with the exception of utilizing the pre-existing STL library. The system offers dual modes of interaction: a user-friendly command-line mode and a batch mode capable of reading from files and delivering outputs to either the terminal or files. The implemented SQL-like statements encompass crucial functionalities such as select, insert, and create operations.

- create table <TABLE_NAME> fields <FIELD_NAME> [, <FIELD_NAME> ...]
- make table <TABLE_NAME> fields <FIELD_NAME> [, <FIELD_NAME> ...]
- insert into <TABLE_NAME> values <VALUE> [, <VALUE> ...]
- select <\* | FIELD_NAME> [, <FIELD_NAME> ...] from <TABLE_NAME> where <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE> [<LOGICAL_OPERATOR> <FIELD_NAME><RELATIONAL_OPERATOR> <VALUE> ...]

## Reserved keywords

- create, make
- table
- fields
- insert
- into
- values
- select
- from
- where
- \*
- LIKE
- and, or
- !=, = , <, >, >=, <=

## Example of Inputs
```
// create table
create table student101 fields  name, 		age, 			major, id

// insert into table
insert into student101 values Daniel, 		18, 			"Computer Engineering", 12345
insert into student101 values "Danny", 	20, 		Sociology, 12346
insert into student101 values Danniella, 21, English                , 12754
insert into student101 values "Dan 12345 Weird Name", 50, 	"MATH Not Meth"     ,    66666
insert into student101 values "Normal guy John",	25, 			"Phys Ed", 00000
insert into student101 values "Another John",	25, 		"Physics", 11111
insert into student101 values "Annie Davis", 31, "Visual Art", 22222
insert into student101 values Anny, 25, Chemistry, 33333

// select
select name, major from student101
select * from student101
select * from student101 where name = Anny
select * from student101 where age < 23
select * from student101 where name = Daniel or ( age >= 19 and age <= 21 )
select * from student101 where (name = Daniel or age >= 19) and age < 21 
select * from student101 where name != Daniel and major != English
select * from student101 where age = 18 and name LIKE Dan
```
## Design

change the repo name:
Can be found in https://github.com/Serenity0204/SQL/blob/master/design.txt

## Features

- it supports unlimitted amount of whitespaces between the input statement
- it supports complex boolean query statement for selection
- it supports prefix like selection using the keyword LIKE
- it supports permanent storage where the created tables will be stored inside the binary files
- it has a interactive mode where the user can type into command line and the input statement can be used to interact with the database
