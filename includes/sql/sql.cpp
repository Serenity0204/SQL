#include "sql.h"

// Constructor
SQL::SQL()
    : _table(Table()), _parser(Parser())
{
}
// Process commands from a file
SQL::SQL(const char* file)
    : SQL()
{
    this->batch(file);
}
// Process a command and return the result table
Table SQL::command(const string& cmd)
{
    Table table;
    string message = "Empty Message to be changed";
    bool error = false;
    this->_process_cmd(cmd, table, message, error);
    this->_table = table;
    this->_error = error;
    return this->_table;
}

void SQL::_process_cmd(const string& cmd, Table& table, string& message, bool& error)
{
    const bool debug = false;

    this->_parser.set_string(cmd);
    MMap<string, string> parsed_tree = this->_parser.parse_tree();
    this->_ptree = parsed_tree;
    if (parsed_tree.empty())
    {
        table = Table();
        message = "Parsing Error";
        error = true;
        return;
    }

    string table_name = parsed_tree["table_name"][0];
    string command = parsed_tree["command"][0];
    string file_name = table_name + ".bin";

    if (command == "create" || command == "make")
    {

        if (file_exists(file_name.c_str()))
        {
            table = Table();
            message = "Table with name:" + table_name + " already exists";
            error = true;
            if (debug) cout << message << endl;
            return;
        }
        vector<string> fields = parsed_tree["fields"];
        table = Table(table_name, fields);
        message = "Created table with name:" + table_name + " success";
        if (debug)
        {
            cout << message << endl;
            cout << "fields:" << fields << endl;
        }
        return;
    }
    if (command == "insert")
    {
        if (!file_exists(file_name.c_str()))
        {
            table = Table();
            message = "Table with name:" + table_name + " does not exist";
            error = true;
            if (debug) cout << message << endl;
            return;
        }
        vector<string> values = parsed_tree["values"];
        table = Table(table_name);
        table.insert_into(values);
        message = "Insert into table with name:" + table_name + " success";
        if (debug)
        {
            cout << message << endl;
            cout << "values::" << values << endl;
        }
        return;
    }
    if (command == "select")
    {
        if (!file_exists(file_name.c_str()))
        {
            table = Table();
            message = "Table with name:" + table_name + " does not exist";
            error = true;
            if (debug) cout << message << endl;
            return;
        }
        bool where_exists = parsed_tree.contains("where");
        // update for select *
        vector<string> selected_fields = parsed_tree["fields"];
        Table root = Table(table_name);
        if (selected_fields[0] == "*") selected_fields = root.get_fields();

        // basic select
        if (!where_exists)
        {
            table = root.select_all(selected_fields);
            message = "Selection with table name:" + table_name + " success";
            if (debug)
            {
                cout << message << endl;
                cout << "selected fields:" << selected_fields << endl;
                cout << root.select_all(selected_fields) << endl;
            }
            return;
        }
        if (where_exists)
        {
            vector<string> condition = parsed_tree["condition"];

            table = root.select(selected_fields, condition);
            message = "Selection with table name:" + table_name + " success";
            if (debug)
            {
                cout << message << endl;
                cout << "selected fields:" << selected_fields << endl;
                cout << "condition:" << condition << endl;
                cout << root.select(selected_fields, condition) << endl;
            }
            return;
        }
        table = Table();
        message = "Invalid command";
        error = true;
        if (debug) cout << message << endl;

        return;
    }
}

void SQL::batch(const char* file, bool file_mode)
{
    ifstream f;
    string temp = file;
    string input_file_path = "../../batch/" + temp;

    f.open(input_file_path.c_str());

    if (f.fail())
    {
        cout << "No file named " << file << " exists." << endl;
        return;
    }
    ofstream o;
    if (file_mode)
    {
#ifdef WIN32
        mkdir("../../batch/output");
#else
        mkdir("../../batch/output", 0755);
#endif
        string output_file = "../../batch/output/output_" + temp;
        o.open(output_file.c_str());
        if (o.fail())
        {
            cout << "cannot find output.txt" << endl;
            return;
        }
    }
    if (!file_mode) cout << "------------------------------Batch Begins------------------------------" << endl;
    if (file_mode) o << "------------------------------Batch Begins------------------------------" << endl;
    while (!f.eof())
    {
        string str;
        getline(f, str);

        if (str[0] == '/')
        {
            if (!file_mode) cout << str << endl;
            if (file_mode) o << str << endl;
            continue;
        }
        if (!str.empty())
        {
            if (!file_mode) cout << "command:" << str << endl;
            if (file_mode) o << "command:" << str << endl;
            Table tb = this->command(str);
            this->_table = tb;

            if (!file_mode)
            {
                cout << tb << endl;
                cout << "records selected: " << this->select_recnos() << endl;
                cout << endl;
                cout << endl;
            }
            if (file_mode)
            {
                o << tb << endl;
                o << "records selected: " << this->select_recnos() << endl;
                o << endl;
                o << endl;
            }
        }
    }

    f.close();
    if (!file_mode) cout << "------------------------------DONE------------------------------" << endl;
    if (file_mode) o << "------------------------------DONE------------------------------" << endl;
    if (file_mode) o.close();
}