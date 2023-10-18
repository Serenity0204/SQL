#include "state_machine_functions.h"

// Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS])
{
    for (int i = 0; i < MAX_ROWS; ++i)
    {
        for (int j = 0; j < MAX_COLUMNS; ++j) _table[i][j] = -1;
    }
}

// Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 1;
}

// Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state)
{
    _table[state][0] = 0;
}

// true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state)
{
    if (_table[state][0] == 1) return true;
    return false;
}

// Mark a range of cells in the array.
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state)
{
    for (int col = from; col <= to; ++col) _table[row][col] = state;
}

// Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state)
{
    int size = strlen(columns);
    for (int index = 0; index < size; ++index)
    {
        int col = columns[index];
        _table[row][col] = state;
    }
}

// Mark this row and column
void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state)
{
    _table[row][column] = state;
}

// This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS])
{
    for (int i = 0; i < MAX_ROWS; ++i)
    {
        for (int j = 0; j < MAX_COLUMNS; ++j) std::cout << _table[i][j] << " ";
        std::cout << std::endl;
        std::cout << std::endl;
    }
}

// show string s and mark this position on the string:
// hello world   pos: 7
//        ^
void show_string(char s[], int _pos)
{
    std::cout << s << std::endl;
    std::string space = "";
    for (int i = 0; i < _pos - 1; ++i) space += " ";
    space += "^";
    std::cout << space << std::endl;
}