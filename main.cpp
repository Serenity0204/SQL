#include "includes/sql/sql.h"

int main(int argc, char* argv[])
{
    SQL sql;
    std::cout << "Type \"end\" to end" << std::endl;
    do
    {
        std::cout << ">>";
        string input = "";
        getline(std::cin, input);
        if (input.empty()) continue;
        if (input == "end") break;
        std::cout << "input:" << input << std::endl;
        Table tb = sql.command(input);
        if (sql.is_error())
        {
            std::cout << "error" << std::endl;
            continue;
        }
        std::cout << tb << std::endl;
        std::cout << "record number:" << sql.select_recnos() << std::endl;
        std::cout << "-------------------------------------------------------------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
    } while (true);
    cout << "DONE" << endl;
    return 0;
}
