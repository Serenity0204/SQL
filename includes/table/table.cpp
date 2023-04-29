#include "table.h"

// for select
Table::Table()
{
    this->_cache = Map<string, MMap<string, long>>();
    this->_field_name_indices = Map<string, long>();
    this->_field_names = vector<string>();
    this->_selected_field_names = vector<string>();
    this->_record_indices = vector<long>();
    this->_to_print = vector<string>();
    this->_n_records = 0;
    this->_table_name = "";
}

// SQL: CREATE TABLE
string Table::create_table(const string& table_name, const vector<string>& field_names)
{
    *this = Table(table_name, field_names);
    return "create " + table_name + " success";
}

// for create
Table::Table(const string& table_name, const vector<string>& field_names)
    : Table()
{
    // if no file exists, create one, if already exists, do nothing
    string table_file = table_name + ".bin";
    string field_file = table_name + "_fields.bin";
    this->_table_name = table_name;
    this->_field_names = field_names;
    

    if (file_exists(table_file.c_str()) || file_exists(field_file.c_str()))
    {
        remove(table_file.c_str());
        remove(field_file.c_str());
    }
    this->_init(true);
    this->_selected_field_names = field_names;
}

// for existing table
Table::Table(const string& table_name)
    : Table()
{
    string table_file = table_name + ".bin";
    string field_file = table_name + "_fields.bin";
    this->_table_name = table_name;
    if (!file_exists(table_file.c_str()) || !file_exists(field_file.c_str())) return;
    this->_init(false);
    this->_index();
}

Table& Table::operator=(const Table& RHS)
{
    if (this == &RHS) return *this;
    this->_table_name = RHS._table_name;
    this->_cache = RHS._cache;
    this->_record_indices = RHS._record_indices;
    this->_to_print = RHS._to_print;
    this->_field_names = RHS._field_names;
    this->_selected_field_names = RHS._selected_field_names;
    this->_n_records = RHS._n_records;
    return *this;
}

// insert
string Table::insert_into(const vector<string>& field_values)
{
    // invalid number of field_values
    if (field_values.size() != this->_field_names.size()) return "Invalid number of arguments";

    // insert data into binary file
    fstream f;
    string table_file = this->_table_name + ".bin";
    open_fileRW(f, table_file.c_str());
    // write it into disk first, then put it into cache
    FileRecord r(field_values);
    long index = r.write(f);

    f.close();
    // write it into cache
    for (int i = 0; i < field_values.size(); ++i)
    {
        // field_names[i]-> field_name, cache[field_name] -> MMap<field_value:index>
        string field_value = field_values[i];
        // MMap.insert(field_value, index), like Map["name"].insert("John", 0)
        this->_cache[this->_field_names[i]].insert(field_value, index);
    }

    // for select_recnos
    this->_record_indices.push_back(index);
    // for print
    this->_to_print += field_values;
    // for number of record
    this->_n_records = this->_record_indices.size();
    return "insert success";
}

// print
ostream& operator<<(ostream& outs, const Table& print_me)
{
    // for field names
    outs << setw(25) << right << "record";
    for (int i = 0; i < print_me._field_names.size(); ++i) outs << setw(25) << right << print_me._field_names[i];

    // for field values
    int endl_index = print_me._field_names.size();
    for (int i = 0; i < print_me._to_print.size(); ++i)
    {
        string field_value = print_me._to_print[i];
        if (i % endl_index == 0)
        {
            outs << endl;
            outs << endl;
            outs << setw(25) << right << i / endl_index;
        }
        outs << setw(25) << right << field_value;
    }
    return outs;
}

// for empty table
void Table::_init(bool is_new)
{
    if (is_new)
    {
        fstream f;
        string table_file = this->_table_name + ".bin";
        string field_file = this->_table_name + "_fields.bin";

        // record the number of fields
        open_fileW(f, field_file.c_str());
        vector<string> mark;
        int size = this->_field_names.size();
        string mark_size = to_string(size);
        mark.push_back(mark_size);
        for (int i = 0; i < this->_field_names.size(); ++i) mark.push_back(this->_field_names[i]);
        FileRecord r(mark);
        r.write(f);
        f.close();

        // create record file
        open_fileW(f, table_file.c_str());
        f.close();
    }
    if (!is_new)
    {
        fstream f;
        FileRecord r;
        string table_file = this->_table_name + ".bin";
        string field_file = this->_table_name + "_fields.bin";

        // determine the fields
        open_fileRW(f, field_file.c_str());
        r.resize(1);
        // get the entry size first
        r.read(f, 0);
        int size = stoi(r.get_records_string()[0]);
        r.resize(size + 1);

        // read the field names
        r.read(f, 0);
        vector<string> field_names = r.get_records_string();
        vector<string>::iterator it = field_names.begin();
        // exclude the size mark
        field_names.erase(it);

        this->_field_names = field_names;
        this->_selected_field_names = field_names;
        f.close();
    }
    this->_record_indices.clear();
    // for field name indices
    for (int i = 0; i < this->_field_names.size(); ++i)
        this->_field_name_indices[this->_field_names[i]] = i;
    // for cache;
    for (int i = 0; i < this->_field_names.size(); ++i)
        this->_cache[this->_field_names[i]] = MMap<string, long>();
}

void Table::_index()
{
    // load field values into cache
    fstream f;
    FileRecord r;
    // resize the vector
    r.resize(this->_field_names.size());
    string table_file = this->_table_name + ".bin";
    open_fileRW(f, table_file.c_str());

    // read every record
    for (long i = 0; r.read(f, i) > 0; ++i)
    {
        // read ith entry
        long bytes = r.read(f, i);
        if (bytes == 0) break;
        // for number of record
        ++this->_n_records;
        // put the ith_entry into cache
        vector<string> ith_entry = r.get_records_string();
        for (int ith_entry_walker = 0; ith_entry_walker < ith_entry.size(); ++ith_entry_walker)
        {
            // this->_field_names[ith_entry_walker] -> stuff like "name"
            // cache[name] -> MMap<field value: indices> like <"John", 0>
            string field_value = ith_entry[ith_entry_walker];
            // assign index to be the read entry index
            long index = i;
            // ex: assign like cache["name"]["John"] += 1
            this->_cache[this->_field_names[ith_entry_walker]].insert(field_value, index);
        }
        // for select_recnos
        this->_record_indices.push_back(i);
        // for print
        this->_to_print += ith_entry;
    }
    f.close();
}

Table Table::select_all(const vector<string>& selected_fields)
{
    const bool debug = false;

    Table temp;
    // the do nothing case
    if (selected_fields.empty() || selected_fields[0] == "*")
    {
        temp = *this;
        return temp;
    }
    // if there's an error
    if (!this->_check_error_fields(selected_fields))
    {
        if (debug) cout << "error fields" << endl;
        return temp;
    }
    // reorder the selected fields
    vector<string> reordered;
    this->_reorder_fields(selected_fields, reordered);
    // copy members
    temp._table_name = this->_table_name;
    temp._field_names = reordered;
    temp._record_indices = this->_record_indices;
    temp._n_records = this->_n_records;

    this->_selected_field_names = selected_fields;
    temp._selected_field_names = selected_fields;
    // get the indices that need to read, no need to copy cache since all the indices are already stored
    this->_read_helper(temp);
    if (debug) cout << temp << endl;
    return temp;
}

void Table::_read_helper(Table& temp)
{
    fstream f;
    string table_file = temp._table_name + ".bin";
    open_fileRW(f, table_file.c_str());
    for (auto idx : temp._record_indices)
    {
        FileRecord r;
        r.resize(this->_field_names.size());
        long byte = r.read(f, idx);

        if (byte == 0) break;
        vector<string> entry = r.get_records_string();
        for (int i = 0; i < entry.size(); ++i)
            if (Helper::is_in(temp._field_names, this->_field_names[i])) temp._to_print += entry[i];
    }
    f.close();
}
vector<long> Table::_select_helper(const string& field_name, const string& op, const string& field_value)
{
    if (!this->_cache.contains(field_name)) return vector<long>();

    // this->_cache[field_name] gives you MMap, use that MMap[field_value] to get the indices
    if (op == "=" && this->_cache[field_name].contains(field_value)) return this->_cache[field_name][field_value];
    if (op == "<=") return this->_bound_helper(this->_cache[field_name].begin(), this->_cache[field_name].upper_bound(field_value));
    if (op == "<") return this->_bound_helper(this->_cache[field_name].begin(), this->_cache[field_name].lower_bound(field_value));
    if (op == ">=") return this->_bound_helper(this->_cache[field_name].lower_bound(field_value), this->_cache[field_name].end());
    if (op == ">") return this->_bound_helper(this->_cache[field_name].upper_bound(field_value), this->_cache[field_name].end());
    // !=, LIKE for todo
    // invalid
    return vector<long>();
}

Table Table::select(const vector<string>& selected_fields, const string& field_name, const string& op, const string& field_value)
{
    const bool debug = false;
    Table temp;

    // select all fields
    bool changed_fields = false;
    if (selected_fields.empty() || selected_fields[0] == "*")
    {
        temp._field_names = this->_field_names;
        changed_fields = true;
    }
    if (!changed_fields && !this->_check_error_fields(selected_fields)) return temp;

    // check if field name exists
    if (!Helper::is_in(this->_field_names, field_name)) return temp;

    // reorder the selected fields
    vector<string> reordered;
    this->_reorder_fields(selected_fields, reordered);
    // copy members
    temp._table_name = this->_table_name;
    temp._field_names = reordered;
    // assign the indices
    temp._record_indices = this->_select_helper(field_name, op, field_value);
    temp._n_records = temp._record_indices.size();

    // read the entries
    this->_read_helper(temp);
    this->_record_indices.clear();
    // assign new record indices
    this->_record_indices = temp._record_indices;
    this->_n_records = temp._n_records;

    this->_selected_field_names = selected_fields;
    temp._selected_field_names = selected_fields;

    if (debug) cout << "n records self:" << this->_n_records << endl;
    if (debug) cout << "n records temp:" << temp._n_records << endl;
    return temp;
}

// accumulate version of select
Table Table::select(const vector<string>& selected_fields, const vector<string>& expression)
{
    const bool debug = false;
    Queue<Token*> infix;
    Helper::generate_tokens(expression, infix);

    ShuntingYard sy(infix);
    Queue<Token*> postfix = sy.postfix();
    Table temp = this->select(selected_fields, postfix);

    if (!infix.empty())
    {
        typename Queue<Token*>::Iterator it;
        for (it = infix.begin(); it != infix.end(); ++it) delete *it;
        infix.clear();
    }
    return temp;
}

Table Table::select(const vector<string>& selected_fields, const Queue<Token*>& expression)
{
    const bool debug = false;
    Table temp;

    bool changed_fields = false;
    // select all fields
    if (selected_fields.empty() || selected_fields[0] == "*")
    {
        temp._field_names = this->_field_names;
        changed_fields = true;
    }
    // if (!changed_fields && !this->_check_error_fields(selected_fields)) return temp;

    // reorder the selected fields
    vector<string> reordered;
    this->_reorder_fields(selected_fields, reordered);
    // copy members
    temp._table_name = this->_table_name;
    temp._field_names = reordered;
    // assign the indices
    if (debug) cout << "indices:" << this->_rpn(expression) << endl;
    temp._record_indices = this->_rpn(expression);
    temp._n_records = temp._record_indices.size();
    if (debug) cout << "n records temp:" << temp._n_records << endl;

    // read the entries
    this->_read_helper(temp);
    this->_record_indices.clear();
    // assign new record indices
    this->_record_indices = temp._record_indices;
    this->_n_records = temp._n_records;
    if (debug) cout << "n records self:" << this->_n_records << endl;

    this->_selected_field_names = selected_fields;
    temp._selected_field_names = selected_fields;

    return temp;
}

vector<long> Table::_rpn(const Queue<Token*>& postfix)
{
    const bool debug = false;
    Stack<Token*> s;
    Queue<Token*> q = postfix;
    Stack<vector<long>> indices;

    while (!q.empty())
    {
        Token* token = q.pop();
        if (token->token_type() == TOKEN_TOKENSTR)
        {
            s.push(token);
            continue;
        }
        // if relational select from cache
        if (token->token_type() == TOKEN_RELATIONAL)
        {
            string relational_op = token->token_string();
            string field_value = s.pop()->token_string(); // field_value first
            string field_name = s.pop()->token_string();  // field_name second
            vector<long> selected_indices = this->_select_helper(field_name, relational_op, field_value);
            if (debug) cout << field_name << relational_op << field_value << " brings:" << selected_indices << endl;
            indices.push(selected_indices);
            // age, 20, <=
            // s: 20
            //    age
            // encounter <= -> pop-> 20, pop->age
        }
        if (token->token_type() == TOKEN_LOGICAL)
        {
            // do set operation
            vector<long> first = indices.pop();
            vector<long> second = indices.pop();

            vector<long> temp;
            temp.clear();

            // if and then do intersection of two indices
            if (token->token_string() == "and")
            {
                temp = Helper::set_intersection(first, second);
                if (debug) cout << "first:" << first << " and second:" << second << " brings:" << temp << endl;
            }
            // then do union
            if (token->token_string() == "or")
            {
                temp = Helper::set_union(first, second);
                if (debug) cout << "first:" << first << " or second:" << second << " brings:" << temp << endl;
            }
            indices.push(temp);
        }
    }
    // rpn error
    if (indices.size() != 1)
    {
        if (debug) cout << "rpn error" << endl;
        return vector<long>();
    }
    if (debug)
    {
        cout << "indices:" << endl;
        cout << indices.top() << endl;
    }
    return indices.top();
}