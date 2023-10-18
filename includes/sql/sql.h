#ifndef SQL_H
#define SQL_H

#include "../../includes/map/mmap.h"
#include "../../includes/parser/parser.h"
#include "../../includes/table/table.h"
#include <cassert>  // assert
#include <fstream>  // ifstream
#include <iostream> // cout, endl
#include <sys/stat.h>
#include <sys/types.h>

class SQL
{
public:
    // Constructor
    SQL();
    // Process commands from a file
    SQL(const char* file);
    // Process a command and return the result table
    Table command(const std::string& cmd);
    // Get all selected record numbers
    std::vector<long> select_recnos() { return this->_table.select_recnos(); }
    // // Print command
    // void print_lookup() { _command.print_lookup(); }
    void batch(const char* file, bool file_mode = false);
    inline bool is_error() { return this->_error; }
    void print_lookup() { this->_ptree.print_lookup(); }
    //======================================
    // TODO: add more methods here as needed
    //======================================
private:
    Table _table;
    Parser _parser;
    bool _error;
    MMap<std::string, std::string> _ptree;
    //======================================
    // TODO: add more methods here as needed
    //======================================
    void _process_cmd(const std::string& cmd, Table& table, std::string& message, bool& error);
};

#endif // SQL_H