#include "includes/sql/sql.h"

int main(int argc, char* argv[])
{
    SQL sql;
    do
    {
        std::cout << ">>";
        string input = "";
        getline(std::cin, input);
        std::cout << input << std::endl;
        if (input.empty()) continue;
        if (input == "end") break;
        std::cout << sql.command(input) << std::endl;
        std::cout << "-------------------------------------------------------------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
    } while (true);
    cout << "DONE" << endl;
    return 0;
}
