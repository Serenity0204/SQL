#ifndef PARSER_H
#define PARSER_H

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "../map/map.h"
#include "../map/mmap.h"
#include "../tokenizer/stokenize.h"
#include "parser_const.h"

class Parser
{
public:
    // Constructors
    Parser();
    Parser(const char*& cstr);
    Parser(const std::string& str);
    // save string to private member
    void set_string(const char*& cstr);
    void set_string(const std::string& str);
    // parse string to map
    MMap<std::string, std::string> parse_tree();
    std::vector<SToken> get_input() { return this->_input; }

    // =====================================
    // TODO: Add more methods here if needed
    // =====================================

private:
    std::vector<SToken> _input;
    std::vector<PARSER_CONST> _types;
    MMap<std::string, std::string> _parse_tree;
    STokenizer _tokenizer;
    int _table[MAX_ROWS][MAX_COLUMNS];
    // helper
    void _tokenize();
    void _init_parse_state();
    bool _parse();
    // =====================================
    // TODO: Add more methods here if needed
    // =====================================
};

#endif // PARSER_H