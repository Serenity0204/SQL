#include "stokenize.h"

STokenizer::STokenizer()
{
    this->_buffer = "";
    this->_pos = 0;
    this->make_table(_table);
}
// set a new string as the input string
void STokenizer::set_string(const char str[])
{
    this->_pos = 0;
    this->_buffer = "";
    int len = strlen(str);
    for (int i = 0; i < len; ++i) this->_buffer += str[i];
}
STokenizer::STokenizer(const char str[])
{
    this->set_string(str);
    this->make_table(_table);
}

bool STokenizer::done() // true: there are no more tokens
{
    if (this->_pos >= this->_buffer.length()) return true;
    return false;
}
bool STokenizer::more() // true: there are more tokens
{
    return !this->done();
}

void STokenizer::make_table(int table[][MAX_COLUMNS])
{
    // init
    state_machine::init_table(this->_table);
    // decide success and fail state
    state_machine::mark_fail(this->_table, 0);
    for (int i = 1; i <= 8; ++i) state_machine::mark_success(_table, i);

    // hard coded state machine
    // init different input for state 0
    state_machine::mark_cells(0, _table, ALPHAS, 1);
    state_machine::mark_cells(0, _table, DIGITS, 2);
    state_machine::mark_cells(0, _table, OPERATORS, 3);
    state_machine::mark_cells(0, _table, COMMA, 4);
    state_machine::mark_cells(0, _table, SPACES, 5);
    state_machine::mark_cells(0, _table, PAREN, 6);
    state_machine::mark_cells(0, _table, STAR, 7);
    state_machine::mark_cell(0, _table, '\"', 8);

    // remains the same state
    state_machine::mark_cells(1, _table, ALPHAS, 1);    // state 1 alpha encounters alpha-> remains in state 1
    state_machine::mark_cells(2, _table, DIGITS, 2);    // state 2 digit encounters digit-> remains in state 2
    state_machine::mark_cells(3, _table, OPERATORS, 3); // state 3 operator encounters operator-> remains in state 3
    for (int i = 0; i <= 255; ++i)                      // for "
        if (i != '\"') state_machine::mark_cell(8, _table, i, 8);
}
// return the type of the token
STRING_TOKEN_TYPES STokenizer::token_type(int state) const
{
    if (state == 1) return TOKEN_ALPHA;
    if (state == 2) return TOKEN_NUMBER;
    if (state == 3) return TOKEN_OPERATOR;
    if (state == 4) return TOKEN_COMMA;
    if (state == 5) return TOKEN_SPACE;
    if (state == 6) return TOKEN_PAREN;
    if (state == 7) return TOKEN_STAR;
    if (state == 8) return TOKEN_QUOTE;
    return TOKEN_UNKNOWN;
}
// extract the longest string that match one of the acceptable token types
bool STokenizer::get_token(int start_state, SToken& token)
{

    /*
    start state -> 0
    extract the first char -> update state
    if state 4,5,6,7 -> true and one corresponding token, update position
    else if state -1 -> true and unknown, update position

    for buffer (start + 1 to end of buffer):
        extract token -> t
        1. state 1:
                if t is ALPHA -> state 1 and pos++
                else terminate
        2. state 2:
                if t is DIGIT -> state 2 and pos++
                else if t == .:
                    extract t and repeat till not digit
                else terminate
        3. state 3:
                if t is OPERATOR -> state 3 and pos++
                else terminate
        else break
    generate token and update pos
    */
    if (this->done()) return false;
    int start = this->_pos;
    int hold = start;

    char t = this->_buffer[this->_pos];
    // check for illegal
    if (!(t <= 255 && t >= 0))
    {
        token = SToken("", TOKEN_UNKNOWN);
        this->_pos++;
        return true;
    }
    int state = 0;
    state = _table[state][t]; // update state

    if (state >= 4 && state <= 7)
    {
        std::string s = "";
        s += t;
        token = SToken(s, token_type(state));
        this->_pos++;
        return true;
    }
    if (state == -1)
    {
        std::string s = "";
        s += t;
        token = SToken(s, TOKEN_UNKNOWN);
        this->_pos++;
        return true;
    }
    // int size = strlen(this->_buffer);
    int size = this->_buffer.length();
    start++;
    // from [start + 1 to size - 1]
    while (start < size)
    {
        t = this->_buffer[start];
        // if illegal between then break
        if (!(t <= 255 && t >= 0)) break;
        int next_state = _table[state][t];
        if (state == 1)
        {
            // still alpha
            if (next_state == 1)
            {
                start++;
                continue;
            }
            break;
        }
        if (state == 2)
        {
            // still digit
            if (next_state == 2)
            {
                start++;
                continue;
            }
            // if t is . and next t exists
            if (t == '.')
            {
                // grab till not digit
                if (start + 1 >= size) break;
                char next_token = this->_buffer[start + 1];
                while (start < size)
                {
                    int next_next_state = _table[state][next_token];
                    if (next_next_state != 2) break;
                    start++;
                    next_token = this->_buffer[start];
                }
                break;
            }
            break;
        }
        if (state == 3)
        {
            // still operator
            if (next_state == 3)
            {
                start++;
                continue;
            }
            break;
        }
        if (state == 4)
        {
            // still operator
            if (next_state == 4)
            {

                start++;
                continue;
            }
            break;
        }
        if (state == 8)
        {
            if (next_state == 8)
            {
                start++;
                continue;
            }
            break;
        }
        break;
    }

    this->_pos = start;
    STRING_TOKEN_TYPES type = token_type(state);
    std::string s = "";
    for (int i = hold; i < start; ++i) s += this->_buffer[i];
    if (type == TOKEN_QUOTE)
    {
        std::string::iterator it = s.begin();
        s.erase(it);
        type = TOKEN_ALPHA;
        this->_pos++;
    }
    token = SToken(s, type);
    return true;
}