#ifndef UTILITIES_H
#define UTILITIES_H

#include <fstream>  // fstream
#include <iostream> // cout, endl

bool file_exists(const char filename[]);
void open_fileRW(std::fstream& f, const char filename[]);
void open_fileW(std::fstream& f, const char filename[]);

#endif // UTILITIES_H