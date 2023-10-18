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
Table SQL::command(const std::string& cmd)
{
    Table table;
    std::string message = "Empty Message to be changed";
    bool error = false;
    this->_process_cmd(cmd, table, message, error);
    this->_table = table;
    this->_error = error;
    return this->_table;
}

void SQL::_process_cmd(const std::string& cmd, Table& table, std::string& message, bool& error)
{
    const bool debug = false;

    this->_parser.set_string(cmd);
    MMap<std::string, std::string> parsed_tree = this->_parser.parse_tree();
    this->_ptree = parsed_tree;
    if (parsed_tree.empty())
    {
        table = Table();
        message = "Parsing Error";
        error = true;
        return;
    }

    std::string table_name = parsed_tree["table_name"][0];
    std::string command = parsed_tree["command"][0];
    std::string file_name = table_name + ".bin";

    if (command == "create" || command == "make")
    {

        if (file_exists(file_name.c_str()))
        {
            table = Table();
            message = "Table with name:" + table_name + " already exists";
            error = true;
            if (debug) std::cout << message << std::endl;
            return;
        }
        std::vector<std::string> fields = parsed_tree["fields"];
        table = Table(table_name, fields);
        message = "Created table with name:" + table_name + " success";
        if (debug)
        {
            std::cout << message << std::endl;
            std::cout << "fields:" << fields << std::endl;
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
            if (debug) std::cout << message << std::endl;
            return;
        }
        std::vector<std::string> values = parsed_tree["values"];
        table = Table(table_name);
        table.insert_into(values);
        message = "Insert into table with name:" + table_name + " success";
        if (debug)
        {
            std::cout << message << std::endl;
            std::cout << "values::" << values << std::endl;
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
            if (debug) std::cout << message << std::endl;
            return;
        }
        bool where_exists = parsed_tree.contains("where");
        // update for select *
        std::vector<std::string> selected_fields = parsed_tree["fields"];
        Table root = Table(table_name);
        if (selected_fields[0] == "*") selected_fields = root.get_fields();

        // basic select
        if (!where_exists)
        {
            table = root.select_all(selected_fields);
            message = "Selection with table name:" + table_name + " success";
            if (debug)
            {
                std::cout << message << std::endl;
                std::cout << "selected fields:" << selected_fields << std::endl;
                std::cout << root.select_all(selected_fields) << std::endl;
            }
            return;
        }
        if (where_exists)
        {
            std::vector<std::string> condition = parsed_tree["condition"];

            table = root.select(selected_fields, condition);
            message = "Selection with table name:" + table_name + " success";
            if (debug)
            {
                std::cout << message << std::endl;
                std::cout << "selected fields:" << selected_fields << std::endl;
                std::cout << "condition:" << condition << std::endl;
                std::cout << root.select(selected_fields, condition) << std::endl;
            }
            return;
        }
        table = Table();
        message = "Invalid command";
        error = true;
        if (debug) std::cout << message << std::endl;

        return;
    }
}

void SQL::batch(const char* file, bool file_mode)
{
    std::ifstream f;
    std::string temp = file;
    std::string input_file_path = "../../batch/" + temp;

    f.open(input_file_path.c_str());

    if (f.fail())
    {
        std::cout << "No file named " << file << " exists." << std::endl;
        return;
    }
    std::ofstream o;
    if (file_mode)
    {
#ifdef WIN32
        mkdir("../../batch/output");
#else
        mkdir("../../batch/output", 0755);
#endif
        std::string output_file = "../../batch/output/output.txt";
        o.open(output_file.c_str());
        if (o.fail())
        {
            std::cout << "cannot find output.txt" << std::endl;
            return;
        }
    }
    if (!file_mode) std::cout << "------------------------------Batch Begins------------------------------" << std::endl;
    if (file_mode) o << "------------------------------Batch Begins------------------------------" << std::endl;
    while (!f.eof())
    {
        std::string str;
        std::getline(f, str);

        if (str[0] == '/')
        {
            if (!file_mode) std::cout << str << std::endl;
            if (file_mode) o << str << std::endl;
            continue;
        }
        if (!str.empty())
        {
            if (!file_mode) std::cout << "command:" << str << std::endl;
            if (file_mode) o << "command:" << str << std::endl;
            Table tb = this->command(str);
            this->_table = tb;

            if (!file_mode)
            {
                std::cout << tb << std::endl;
                std::cout << "records selected: " << this->select_recnos() << std::endl;
                std::cout << std::endl;
                std::cout << std::endl;
            }
            if (file_mode)
            {
                o << tb << std::endl;
                o << "records selected: " << this->select_recnos() << std::endl;
                o << std::endl;
                o << std::endl;
            }
        }
    }

    f.close();
    if (!file_mode) std::cout << "------------------------------DONE------------------------------" << std::endl;
    if (file_mode) o << "------------------------------DONE------------------------------" << std::endl;
    if (file_mode) o.close();
}