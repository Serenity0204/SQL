#include "parser.h"

Parser::Parser()
{
    this->_parse_tree = MMap<string, string>();
    this->_input = vector<SToken>();
    this->_types = vector<PARSER_CONST>();
    this->_tokenizer = STokenizer();
    this->_init_parse_state();
}
Parser::Parser(const string& str)
    : Parser()
{
    this->_tokenizer.set_string(str.c_str());
    this->_tokenize();
    this->_init_parse_state();
}
// save string to private member
void Parser::set_string(const char*& cstr)
{
    this->_parse_tree.clear();
    this->_types.clear();
    this->_input.clear();
    this->_tokenizer.set_string(cstr);
    this->_tokenize();
}
void Parser::set_string(const string& str)
{
    this->_parse_tree.clear();
    this->_types.clear();
    this->_input.clear();
    this->_tokenizer.set_string(str.c_str());
    this->_tokenize();
}
// parse string to map
MMap<string, string> Parser::parse_tree()
{
    bool success = this->_parse();
    if (!success)
    {
        this->_parse_tree.clear();
        return this->_parse_tree;
    }
    return this->_parse_tree;
}

bool Parser::_parse()
{
    // wrong input
    if (this->_types.size() != this->_input.size()) return false;

    int state = 0;
    PARSER_CONST type = this->_types[0];
    bool valid_cmd = false;
    bool did_where = false;
    if (type == CREATE)
    {
        this->_parse_tree.insert("command", this->_input[0].token_str());
        this->_parse_tree.insert_key("fields");
        this->_parse_tree.insert_key("table_name");
        valid_cmd = true;
    }
    if (type == INSERT)
    {
        this->_parse_tree.insert("command", "insert");
        this->_parse_tree.insert_key("values");
        this->_parse_tree.insert_key("table_name");
        valid_cmd = true;
    }
    if (type == SELECT)
    {
        this->_parse_tree.insert("command", "select");
        this->_parse_tree.insert_key("condition");
        this->_parse_tree.insert_key("fields");
        this->_parse_tree.insert_key("table_name");
        this->_parse_tree.insert_key("where");
        valid_cmd = true;
    }
    // not valid cmd
    if (!valid_cmd) return false;

    state = this->_table[0][type];
    // cout << "table(" << state << "," << type << ")" << endl;

    for (int i = 1; i < this->_input.size(); ++i)
    {

        type = this->_types[i];
        int prev_state = state;
        state = this->_table[prev_state][type];
        if (state == -1) return false;

        // dump keywords and add in literals
        if (state == 3 || state == 9 || state == 18) this->_parse_tree["table_name"] += this->_input[i].token_str(); // table name in state 3 for create, and state 9 for insert
        if (state == 5 || state == 16 || state == 14) this->_parse_tree["fields"] += this->_input[i].token_str();
        if (state == 11) this->_parse_tree["values"] += this->_input[i].token_str();
        if (state == 19 && !did_where)
        {
            did_where = true;
            this->_parse_tree["where"] += string("yes");
        }
        if (state >= 19 && state <= 24 && this->_input[i].token_str() != "where") this->_parse_tree["condition"] += this->_input[i].token_str();
    }
    // // return true;
    if (this->_parse_tree["condition"].empty()) this->_parse_tree.erase("condition");
    if (this->_parse_tree["where"].empty()) this->_parse_tree.erase("where");
    return is_success(this->_table, state);
}

// hard code state machine here
void Parser::_init_parse_state()
{
    init_table(this->_table);
    // mark success state
    for (int i = 0; i <= 24; ++i)
    {
        if (i == 5 || i == 11 || i == 18 || i == 23 || i == 24)
        {
            mark_success(this->_table, i);
            continue;
        }
        mark_fail(this->_table, i);
    }
    // create table <TABLE_NAME> fields <FIELD_NAME> [, <FIELD_NAME> ...]

    // insert into <TABLE_NAME> values <VALUE> [, <VALUE> ...]

    // select <* | FIELD_NAME> [, <FIELD_NAME> ...]
    // from <TABLE_NAME>
    // where <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE> [<LOGICAL_OPERATOR> <FIELD_NAME> <RELATIONAL_OPERATOR> <VALUE> ...]

    // state 1 to 6 for insert
    // create
    mark_cell(0, this->_table, CREATE, 1);  // create, failed state
    mark_cell(1, this->_table, TABLE, 2);   // table, failed state
    mark_cell(2, this->_table, LITERAL, 3); // table name, failed state
    mark_cell(3, this->_table, FIELDS, 4);  // fields, failed state
    mark_cell(4, this->_table, LITERAL, 5); // field name, success state
    mark_cell(5, this->_table, COMMAS, 6);  // comma, failed state
    mark_cell(6, this->_table, LITERAL, 5); // if after comma, receive a literal, go back to state 5, success state

    // state 7 to 12
    // insert
    mark_cell(0, this->_table, INSERT, 7);    // insert, failed state
    mark_cell(7, this->_table, INTO, 8);      // into, failed state
    mark_cell(8, this->_table, LITERAL, 9);   // table name, failed state
    mark_cell(9, this->_table, VALUES, 10);   // values, failed state
    mark_cell(10, this->_table, LITERAL, 11); // value, success state
    mark_cell(11, this->_table, COMMAS, 12);  // comma, failed state
    mark_cell(12, this->_table, LITERAL, 11); // if after comma, receive a literal, go back to state 11, success state

    // select, leave paren issues to shunting yard
    mark_cell(0, this->_table, SELECT, 13);    // select, failed state
    mark_cell(13, this->_table, ASTERISK, 16); // *, failed state
    mark_cell(16, this->_table, FROM, 17);     // select * from, failed

    mark_cell(13, this->_table, LITERAL, 14); // field name, failed state
    mark_cell(14, this->_table, COMMAS, 15);  // after field_name a comma, failed state
    mark_cell(15, this->_table, LITERAL, 14); // after comma a field_name, failed state
    mark_cell(14, this->_table, FROM, 17);    // select field_name, ... field_name from, failed state

    mark_cell(17, this->_table, LITERAL, 18); // table name, success state

    mark_cell(18, this->_table, WHERE, 19);   // where, failed state
    mark_cell(19, this->_table, LITERAL, 21); // where field_name, failed state

    mark_cell(19, this->_table, PARENS, 20);  // where (, failed state
    mark_cell(20, this->_table, LITERAL, 21); // where (field_name, failed state
    mark_cell(20, this->_table, PARENS, 20);  // where ((field_name, failed state

    mark_cell(21, this->_table, RELATIONAL, 22); // where (field_name =, failed state
    mark_cell(22, this->_table, LITERAL, 23);    // where field_name = John, success state

    mark_cell(23, this->_table, LOGICAL, 19); // where field_name = John and, failed state
    mark_cell(23, this->_table, PARENS, 24);  // where (field_name = John), success state
    mark_cell(24, this->_table, LOGICAL, 19); // where (field_name = John) and , failed state
    mark_cell(24, this->_table, PARENS, 24);  // where (field_name = John) and , failed state
}

// tokenize string into vector
void Parser::_tokenize()
{
    while (this->_tokenizer.more())
    {
        SToken token = SToken();
        this->_tokenizer >> token;
        if (token.type() == TOKEN_UNKNOWN && token.token_str() != "\"") continue;
        if (token.type() == TOKEN_SPACE) continue;
        // reserved keywords
        bool did_push = false;
        if (token.type() == TOKEN_ALPHA)
        {
            int prev_size = this->_types.size();
            if (token.token_str() == "create" || token.token_str() == "make") this->_types.push_back(CREATE);
            if (token.token_str() == "table") this->_types.push_back(TABLE);
            if (token.token_str() == "fields") this->_types.push_back(FIELDS);
            if (token.token_str() == "insert") this->_types.push_back(INSERT);
            if (token.token_str() == "into") this->_types.push_back(INTO);
            if (token.token_str() == "values") this->_types.push_back(VALUES);
            if (token.token_str() == "select") this->_types.push_back(SELECT);
            if (token.token_str() == "from") this->_types.push_back(FROM);
            if (token.token_str() == "where") this->_types.push_back(WHERE);

            if (prev_size != this->_types.size()) did_push = true;
        }

        // logical and relational
        if (token.type() == TOKEN_ALPHA && (token.token_str() == "or" || token.token_str() == "and"))
        {
            did_push = true;
            this->_types.push_back(LOGICAL);
        }
        // =, !=, <, >, <= and >=
        bool is_relational = (token.token_str() == "!=" || token.token_str() == "<=" || token.token_str() == ">=" || token.token_str() == "=" || token.token_str() == ">" || token.token_str() == "<");
        if (token.type() == TOKEN_OPERATOR && is_relational)
        {
            // cout << "op:" << token.token_str() << endl;
            this->_types.push_back(RELATIONAL);
        }
        if (token.type() == TOKEN_ALPHA && token.token_str() == "LIKE")
        {
            did_push = true;
            this->_types.push_back(RELATIONAL);
        }

        if (token.type() == TOKEN_PAREN) this->_types.push_back(PARENS);
        if (token.type() == TOKEN_STAR && token.token_str() == "*") this->_types.push_back(ASTERISK);
        if (token.type() == TOKEN_COMMA && token.token_str() == ",") this->_types.push_back(COMMAS);
        if (!did_push && (token.type() == TOKEN_ALPHA || token.type() == TOKEN_NUMBER)) this->_types.push_back(LITERAL);
        if (token.type() == TOKEN_UNKNOWN && token.token_str() == "\"") this->_types.push_back(QUOTE);
        this->_input.push_back(token);
    }
    // cout << _types << endl;
    // cout << _input << endl;
}