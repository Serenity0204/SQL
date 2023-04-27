#ifndef PARSER_CONST_H
#define PARSER_CONST_H

// create table <TABLE_NAME> fields <FIELD_NAME> [, <FIELD_NAME> ...]

// insert into <TABLE_NAME> values <VALUE> [, <VALUE> ...]

// select <* | FIELD_NAME> [, <FIELD_NAME> ...]
// from <TABLE_NAME>
// where <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE> [<LOGICAL_OPERATOR> <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE> ...]

enum PARSER_CONST
{
    CREATE = 0,
    TABLE = 1,
    FIELDS = 2,
    INSERT = 3,
    INTO = 4,
    VALUES = 5,
    SELECT = 6,
    ASTERISK = 7,
    FROM = 8,
    WHERE = 9,
    LITERAL = 10,
    COMMAS = 11,
    PARENS = 12,
    RELATIONAL = 13,
    LOGICAL = 14,
    QUOTE = 15,
};

#endif // PARSER_CONST_H