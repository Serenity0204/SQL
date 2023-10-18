#ifndef STOKENIZE_H
#define STOKENIZE_H
#pragma once

#include <cassert>
#include <cstring>
#include <iostream>

#include "constants.h"
#include "state_machine_functions.h"
#include "token.h"

class STokenizer
{
private:
    // char _buffer[MAX_BUFFER]; // input string
    std::string _buffer;
    int _pos; // current position in the string
    int _table[MAX_ROWS][MAX_COLUMNS];

    // create table for all the tokens we will recognize (e.g. doubles, words, etc.)
    /*
    const char ALPHAS[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char DIGITS[] = "0123456789";
    const char OPERATORS[] = "><=!+-%&|"; // remove +-%& later
    const char SPACES[] = {' ', '\t', '\n', '\0'};
    const char PUNCS[] = "?.,:;'`~!";
    */
    void make_table(int table[][MAX_COLUMNS]);
    // return the type of the token
    STRING_TOKEN_TYPES token_type(int state) const;
    // extract the longest string that match one of the acceptable token types
    bool get_token(int start_state, SToken& token);

    // =====================================
    // TODO: Add more methods here if needed
    // =====================================
public:
    // Constructors
    STokenizer();
    // set a new string as the input string
    void set_string(const char str[]);

    STokenizer(const char str[]);

    bool done(); // true: there are no more tokens

    bool more(); // true: there are more tokens

    // extract one token (very similar to the way cin >> works)
    friend STokenizer& operator>>(STokenizer& s, SToken& t)
    {
        int state = 0;
        s.get_token(0, t);
        return s;
    }
    // =====================================
    // TODO: Add more methods here if needed
    // =====================================
};

#endif // STOKENIZE_H