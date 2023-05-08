#include "includes/sql/sql.h"

int main(int argc, char* argv[])
{
    SQL sql;
    std::cout << "--------------------------------------------------------WELCOME--------------------------------------------------------" << std::endl;
    do
    {
        std::cout << "Type \"end\" to end" << std::endl;
        std::cout << ">>";
        string input = "";
        getline(std::cin, input);
        std::cout << endl;

        if (input.empty()) continue;
        if (input == "cls")
        {
            system("cls");
            continue;
        }
        if (input == "prep")
        {
            remove("student101.bin");
            remove("student101_fields.bin");
            sql.batch("prepared_data.txt", true);
            continue;
        }
        if (input == "end") break;
        std::cout << "input:" << input << std::endl;
        Table tb = sql.command(input);
        if (sql.is_error())
        {
            std::cout << "error" << std::endl;
            continue;
        }
        std::cout << "selected fields:" << tb.get_fields() << std::endl;
        std::cout << tb << std::endl;
        std::cout << "record number:" << sql.select_recnos() << std::endl;
        std::cout << "-------------------------------------------------------------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
    } while (true);
    std::cout << "--------------------------------------------------------DONE--------------------------------------------------------" << std::endl;
    return 0;
}
