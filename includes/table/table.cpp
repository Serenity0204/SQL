#include "table.h"

// for select
Table::Table()
{
    this->_cache = Map<std::string, MMap<std::string, long>>();
    this->_field_name_indices = Map<std::string, long>();
    this->_field_names = std::vector<std::string>();
    this->_selected_field_names = std::vector<std::string>();
    this->_record_indices = std::vector<long>();
    this->_to_print = std::vector<std::string>();
    this->_n_records = 0;
    this->_table_name = "";
    this->_total_records = 0;
}

// SQL: CREATE TABLE
std::string Table::create_table(const std::string& table_name, const std::vector<std::string>& field_names)
{
    *this = Table(table_name, field_names);
    return "create " + table_name + " success";
}

// for create
Table::Table(const std::string& table_name, const std::vector<std::string>& field_names)
    : Table()
{
    // if no file exists, create one, if already exists, do nothing
    std::string table_file = table_name + ".bin";
    std::string field_file = table_name + "_fields.bin";
    this->_table_name = table_name;
    this->_field_names = field_names;
    this->_selected_field_names = field_names;
    if (file_exists(table_file.c_str()) || file_exists(field_file.c_str()))
    {
        remove(table_file.c_str());
        remove(field_file.c_str());
    }
    this->_init(true);
    this->_selected_field_names = field_names;
}

// for existing table
Table::Table(const std::string& table_name)
    : Table()
{
    std::string table_file = table_name + ".bin";
    std::string field_file = table_name + "_fields.bin";
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
    this->_total_records = RHS._total_records;
    return *this;
}

// insert
std::string Table::insert_into(const std::vector<std::string>& field_values)
{
    // invalid number of field_values
    if (field_values.size() != this->_field_names.size()) return "Invalid number of arguments";

    // insert data into binary file
    std::fstream f;
    std::string table_file = this->_table_name + ".bin";
    open_fileRW(f, table_file.c_str());
    // write it into disk first, then put it into cache
    FileRecord r(field_values);
    long index = r.write(f);

    f.close();
    // write it into cache
    for (int i = 0; i < field_values.size(); ++i)
    {
        // field_names[i]-> field_name, cache[field_name] -> MMap<field_value:index>
        std::string field_value = field_values[i];
        // MMap.insert(field_value, index), like Map["name"].insert("John", 0)
        this->_cache[this->_field_names[i]].insert(field_value, index);
    }

    // for select_recnos
    this->_record_indices.push_back(index);
    // for print
    this->_to_print += field_values;
    // for number of record
    this->_n_records = this->_record_indices.size();
    this->_total_records++;
    return "insert success";
}

// print
std::ostream& operator<<(std::ostream& outs, const Table& print_me)
{
    // for field names
    outs << std::setw(25) << std::right << "record";
    for (int i = 0; i < print_me._field_names.size(); ++i) outs << std::setw(25) << std::right << print_me._field_names[i];

    // for field values
    int endl_index = print_me._field_names.size();
    for (int i = 0; i < print_me._to_print.size(); ++i)
    {
        std::string field_value = print_me._to_print[i];
        if (i % endl_index == 0)
        {
            outs << std::endl;
            outs << std::endl;
            outs << std::setw(25) << std::right << i / endl_index;
        }
        outs << std::setw(25) << std::right << field_value;
    }
    return outs;
}

// for empty table
void Table::_init(bool is_new)
{
    if (is_new)
    {
        std::fstream f;
        std::string table_file = this->_table_name + ".bin";
        std::string field_file = this->_table_name + "_fields.bin";

        // record the number of fields
        open_fileW(f, field_file.c_str());
        std::vector<std::string> mark;
        int size = this->_field_names.size();
        std::string mark_size = std::to_string(size);
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
        std::fstream f;
        FileRecord r;
        std::string table_file = this->_table_name + ".bin";
        std::string field_file = this->_table_name + "_fields.bin";

        // determine the fields
        open_fileRW(f, field_file.c_str());
        r.resize(1);
        // get the entry size first
        r.read(f, 0);
        int size = stoi(r.get_records_string()[0]);
        r.resize(size + 1);

        // read the field names
        r.read(f, 0);
        std::vector<std::string> field_names = r.get_records_string();
        std::vector<std::string>::iterator it = field_names.begin();
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
        this->_cache[this->_field_names[i]] = MMap<std::string, long>();
}

void Table::_index()
{
    // load field values into cache
    std::fstream f;
    FileRecord r;
    // resize the vector
    r.resize(this->_field_names.size());
    std::string table_file = this->_table_name + ".bin";
    open_fileRW(f, table_file.c_str());

    // read every record
    for (long i = 0; r.read(f, i) > 0; ++i)
    {
        // read ith entry
        long bytes = r.read(f, i);
        if (bytes == 0) break;
        // put the ith_entry into cache
        std::vector<std::string> ith_entry = r.get_records_string();
        for (int ith_entry_walker = 0; ith_entry_walker < ith_entry.size(); ++ith_entry_walker)
        {
            // this->_field_names[ith_entry_walker] -> stuff like "name"
            // cache[name] -> MMap<field value: indices> like <"John", 0>
            std::string field_value = ith_entry[ith_entry_walker];
            // assign index to be the read entry index
            long index = i;
            // ex: assign like cache["name"]["John"] += 1
            this->_cache[this->_field_names[ith_entry_walker]].insert(field_value, index);
        }
        // for select_recnos
        this->_record_indices.push_back(i);
        // for print
        this->_to_print += ith_entry;
        this->_total_records++;
    }
    f.close();
}

Table Table::select_all(const std::vector<std::string>& selected_fields)
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
        if (debug) std::cout << "error fields" << std::endl;
        return temp;
    }
    // reorder the selected fields
    std::vector<std::string> reordered;
    this->_reorder_fields(selected_fields, reordered);
    // copy members
    temp._table_name = this->_table_name;
    temp._field_names = reordered;
    this->_record_indices.clear();
    for (int i = 0; i < this->_total_records; ++i) this->_record_indices.push_back(i);
    temp._record_indices = this->_record_indices;
    temp._n_records = this->_n_records;
    temp._total_records = this->_total_records;

    this->_selected_field_names = selected_fields;
    temp._selected_field_names = selected_fields;
    // get the indices that need to read, no need to copy cache since all the indices are already stored
    this->_read_helper(temp);
    if (debug) std::cout << temp << std::endl;
    return temp;
}

void Table::_read_helper(Table& temp)
{
    std::fstream f;
    std::string table_file = temp._table_name + ".bin";
    open_fileRW(f, table_file.c_str());
    for (auto idx : temp._record_indices)
    {
        FileRecord r;
        r.resize(this->_field_names.size());
        long byte = r.read(f, idx);

        if (byte == 0) break;
        std::vector<std::string> entry = r.get_records_string();
        for (int i = 0; i < entry.size(); ++i)
            if (Helper::is_in(temp._field_names, this->_field_names[i])) temp._to_print += entry[i];
    }
    f.close();
}
std::vector<long> Table::_select_helper(const std::string& field_name, const std::string& op, const std::string& field_value)
{
    if (!this->_cache.contains(field_name)) return std::vector<long>();
    // cout << "op:" << op << endl;
    // this->_cache[field_name] gives you MMap, use that MMap[field_value] to get the indices
    if (op == "=" && this->_cache[field_name].contains(field_value)) return this->_cache[field_name][field_value];
    if (op == "<=") return this->_bound_helper(this->_cache[field_name].begin(), this->_cache[field_name].upper_bound(field_value));
    if (op == "<") return this->_bound_helper(this->_cache[field_name].begin(), this->_cache[field_name].lower_bound(field_value));
    if (op == ">=") return this->_bound_helper(this->_cache[field_name].lower_bound(field_value), this->_cache[field_name].end());
    if (op == ">") return this->_bound_helper(this->_cache[field_name].upper_bound(field_value), this->_cache[field_name].end());
    // !=, LIKE for todo
    if (op == "!=")
    {
        std::vector<long> res;
        typename MMap<std::string, long>::Iterator it;
        for (it = this->_cache[field_name].begin(); it != this->_cache[field_name].end(); ++it)
        {
            // cout << (*it).key << "?" << field_name << endl;
            if ((*it).key == field_value) continue;
            res += (*it).value_list;
        }
        return res;
    }
    if (op == "LIKE")
    {
        std::vector<long> res;
        Trie trie;
        std::set<std::string> keys;
        typename MMap<std::string, long>::Iterator it;
        for (it = this->_cache[field_name].begin(); it != this->_cache[field_name].end(); ++it) trie.insert((*it).key);
        std::vector<std::string> prefix = trie.get_prefix(field_value);
        for (it = this->_cache[field_name].begin(); it != this->_cache[field_name].end(); ++it)
        {
            std::string key = (*it).key;
            if (Helper::is_in(prefix, key)) res += (*it).value_list;
        }
        return res;
    }
    // invalid
    return std::vector<long>();
}

Table Table::select(const std::vector<std::string>& selected_fields, const std::string& field_name, const std::string& op, const std::string& field_value)
{
    const bool debug = false;
    Table temp;

    // select all fields
    bool changed_fields = false;
    if (selected_fields.empty() || selected_fields[0] == "*")
    {
        temp._field_names = this->get_original_fields();
        temp._selected_field_names = this->get_original_fields();
        this->_field_names = this->get_original_fields();
        this->_selected_field_names = this->get_original_fields();
        changed_fields = true;
    }
    if (!selected_fields.empty() && selected_fields[0] != "*")
    {
        this->_selected_field_names = selected_fields;
        temp._selected_field_names = selected_fields;
    }

    if (!changed_fields && !this->_check_error_fields(selected_fields)) return temp;

    // check if field name exists
    if (!Helper::is_in(this->_field_names, field_name)) return temp;

    // reorder the selected fields
    std::vector<std::string> reordered;
    std::vector<std::string> selected_temp = this->_selected_field_names;
    this->_reorder_fields(selected_temp, reordered);
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

    if (debug) std::cout << "n records self:" << this->_n_records << std::endl;
    if (debug) std::cout << "n records temp:" << temp._n_records << std::endl;

    return temp;
}

// accumulate version of select
Table Table::select(const std::vector<std::string>& selected_fields, const std::vector<std::string>& expression)
{
    const bool debug = false;
    Queue<Token*> infix;
    Helper::generate_tokens(expression, infix);

    ShuntingYard sy(infix);
    Queue<Token*> postfix = sy.postfix();
    if (debug) std::cout << "postfix:" << postfix << std::endl;
    Table temp = this->select(selected_fields, postfix);

    if (!infix.empty())
    {
        typename Queue<Token*>::Iterator it;
        for (it = infix.begin(); it != infix.end(); ++it)
        {
            switch ((*it)->token_type())
            {
            case TOKEN_TOKENSTR:
                delete static_cast<TokenStr*>((*it));
                break;
            case TOKEN_RIGHTPAREN:
                delete static_cast<RightParen*>((*it));
                break;
            case TOKEN_LEFTPAREN:
                delete static_cast<LeftParen*>((*it));
                break;
            case TOKEN_LOGICAL:
                delete static_cast<Logical*>((*it));
                break;
            case TOKEN_RELATIONAL:
                delete static_cast<Relational*>((*it));
                break;
            default:
                break;
            }
        }
        infix.clear();
    }
    return temp;
}

Table Table::select(const std::vector<std::string>& selected_fields, const Queue<Token*>& expression)
{
    const bool debug = false;
    Table temp;

    bool changed_fields = false;
    // select all fields
    if (selected_fields.empty() || selected_fields[0] == "*")
    {

        this->_field_names = this->get_original_fields();
        this->_selected_field_names = this->get_original_fields();
        temp._field_names = this->get_original_fields();
        temp._selected_field_names = this->get_original_fields();
        if (debug)
        {
            std::cout << std::endl;
            std::cout << "self changed to:" << this->_selected_field_names << std::endl;
            std::cout << "temp changed to:" << temp._selected_field_names << std::endl;
        }
        changed_fields = true;
    }
    if (!selected_fields.empty() && selected_fields[0] != "*")
    {
        this->_selected_field_names = selected_fields;
        temp._selected_field_names = selected_fields;
    }

    if (!changed_fields && !this->_check_error_fields(selected_fields)) return temp;

    // reorder the selected fields
    std::vector<std::string> reordered;
    std::vector<std::string> selected_temp = this->_selected_field_names;
    this->_reorder_fields(selected_temp, reordered);
    // copy members
    temp._table_name = this->_table_name;
    temp._field_names = reordered;
    // assign the indices
    if (debug) std::cout << "indices:" << this->_rpn(expression) << std::endl;
    if (!expression.empty()) temp._record_indices = this->_rpn(expression);
    if (expression.empty())
    {
        temp = this->select_all(temp._field_names);
        return temp;
    }
    temp._n_records = temp._record_indices.size();
    if (debug) std::cout << "n records temp:" << temp._n_records << std::endl;

    // read the entries
    this->_read_helper(temp);
    this->_record_indices.clear();
    // assign new record indices
    this->_record_indices = temp._record_indices;
    this->_n_records = temp._n_records;
    if (debug) std::cout << "n records self:" << this->_n_records << std::endl;

    return temp;
}

std::vector<long> Table::_rpn(const Queue<Token*>& postfix)
{
    const bool debug = false;
    Stack<Token*> s;
    Queue<Token*> q = postfix;
    Stack<std::vector<long>> indices;
    if (debug) std::cout << q << std::endl;

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
            std::string relational_op = token->token_string();
            std::string field_value = s.pop()->token_string(); // field_value first
            std::string field_name = s.pop()->token_string();  // field_name second
            std::vector<long> selected_indices = this->_select_helper(field_name, relational_op, field_value);
            // cout << "indices:" << selected_indices << endl;
            if (debug) std::cout << field_name << relational_op << field_value << " brings:" << selected_indices << std::endl;
            indices.push(selected_indices);
            // age, 20, <=
            // s: 20
            //    age
            // encounter <= -> pop-> 20, pop->age
        }
        if (token->token_type() == TOKEN_LOGICAL)
        {
            // do set operation
            std::vector<long> first = indices.pop();
            std::vector<long> second = indices.pop();

            std::vector<long> temp;
            temp.clear();

            // if and then do intersection of two indices
            if (token->token_string() == "and")
            {
                temp = Helper::set_intersection(first, second);
                if (debug) std::cout << "first:" << first << " and second:" << second << " brings:" << temp << std::endl;
            }
            // then do union
            if (token->token_string() == "or")
            {
                temp = Helper::set_union(first, second);
                if (debug) std::cout << "first:" << first << " or second:" << second << " brings:" << temp << std::endl;
            }
            indices.push(temp);
        }
    }
    // rpn error
    if (indices.size() != 1)
    {
        if (debug) std::cout << "rpn error" << std::endl;
        if (debug) std::cout << indices << std::endl;
        return std::vector<long>();
    }
    if (debug)
    {
        std::cout << "indices:" << std::endl;
        std::cout << indices.top() << std::endl;
    }
    return indices.top();
}