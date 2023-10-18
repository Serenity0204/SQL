#ifndef FTOKENIZE_H
#define FTOKENIZE_H
#pragma once

#include <fstream>
#include <iostream>

#include "constants.h"
#include "stokenize.h"
#include "token.h"

class FTokenizer
{
private:
    std::ifstream _f; // file being tokenized
    STokenizer _stk;  // STokenizer object to tokenize current block
    int _pos;         // current position in the file
    int _blockPos;    // current position in the current block
    bool _more;       // false if last token of the last block
                      //   has been processed and now we are at
                      //   the end of the last block.

    bool get_new_block(); // gets the new block from the file

    // =====================================
    // TODO: Add more methods here if needed
    // =====================================
public:
    // Maximum number of characters in a block
    const int MAX_BLOCK = MAX_BUFFER;
    // Constructor
    FTokenizer(char* fname);
    // Extract one token (very similar to the way cin >> works)
    SToken next_token();
    bool more() { return this->_more; }         // returns the current value of _more
    int pos() { return this->_pos; }            // returns the value of _pos
    int block_pos() { return this->_blockPos; } // returns the value of _blockPos
    friend FTokenizer& operator>>(FTokenizer& f, SToken& t)
    {
        t = f.next_token();
        return f;
    }

    // =====================================
    // TODO: Add more methods here if needed
    // =====================================
};

#endif // FTOKENIZE_H