# SQL

A naive version of implementation of relational database including a SQL like language for interacting with the database itself.
most of the data structures used in this project are implemented from scratch, with the exception of std::set and std::vector. This
project supports two modes to interact with the database, the first one is command line mode, the second one is batch mode(read from files).
The supported sql like statements include select, insert, and create.

- create table <TABLE_NAME> fields <FIELD_NAME> [, <FIELD_NAME> ...]
- insert into <TABLE_NAME> values <VALUE> [, <VALUE> ...]
- select <\* | FIELD_NAME> [, <FIELD_NAME> ...] from <TABLE_NAME> where <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE> [<LOGICAL_OPERATOR> <FIELD_NAME><RELATIONAL_OPERATOR> <VALUE> ...]

## Reserved keywords

- create
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
- !=, = , <, >, >=, <=

## Example of Inputs

- create table employee fields last_name, first_name, major
- insert into employee values Blow, Joe, CS
- insert into employee values "Jackson", Billy, Math
- select first_name, last_name from student
- select \* from student
- select \* from student where last_name
- select \* from student where age < 23
- select \* from student where last_name = Yang and age < 23
- select \* from student where (last_name = Yang or major = CS) and age < 23
- select \* from student where not last_name = Yang and not major = CS
- select \* from student where age = 18 and name LIKE Dan

## Design

change the repo name:
Can be found in https://github.com/Serenity0204/SQL/blob/master/design.txt

## Features

- it supports unlimitted amount of whitespaces between the input statement
- it supports complex boolean query statement for selection
- it supports prefix like selection using the keyword LIKE
- it supports permanent storage where the created tables will be stored inside the binary files
- it has a interactive mode where the user can type into command line and the input statement can be used to interact with the database
