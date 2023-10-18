#include "utilities.h"

bool file_exists(const char filename[])
{
    const bool debug = false;
    std::fstream ff;
    // Open the file
    ff.open(filename, std::fstream::in | std::fstream::binary);
    if (ff.fail())
    {
        if (debug) std::cout << "FAILED: file_exists(): File does NOT exist: " << filename << std::endl;
        return false;
    }
    if (debug) std::cout << "file_exists(): File DOES exist: " << filename << std::endl;
    // Close the file
    ff.close();
    return true;
}

void open_fileRW(std::fstream& f, const char filename[])
{
    const bool debug = false;
    // Opening a nonexistent file for in|out|app causes a fail()
    // so, if the file does not exist, create it by opening it for output
    if (!file_exists(filename))
    {
        // Create the file
        f.open(filename, std::fstream::out | std::fstream::binary);
        if (f.fail())
        {
            std::cout << "FAILED: open_fileRW(): File does NOT exist: " << filename << std::endl;
            return;
        }
        if (debug) std::cout << "open_fileRW(): File created successfully: " << filename << std::endl;
        return;
    }
    // Open the file
    f.open(filename, std::fstream::in | std::fstream::out | std::fstream::binary);
    if (f.fail())
    {
        std::cout << "FAILED: open_fileRW(): File does NOT exist: " << filename << std::endl;
        return;
    }
}

void open_fileW(std::fstream& f, const char filename[])
{
    // Open the file
    f.open(filename, std::fstream::out | std::fstream::binary);
    if (f.fail())
    {
        std::cout << "FAILED: open_fileW(): File does NOT exist: " << filename << std::endl;
        return;
    }
    // Keep opening the file for writing
}