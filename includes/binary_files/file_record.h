#ifndef FILE_RECORD_H
#define FILE_RECORD_H
#pragma once

#include <cstring>  // strncpy
#include <fstream>  // fstream
#include <iomanip>  // setw, right
#include <iostream> // cout, endl
#include <memory>
#include <vector> // vector

class FileRecord
{

private:
    // The maximum size of the record
    static const int MAX = 100;
    // The record vector
    std::vector<std::string> _records;

public:
    // When construct a FileRecord, it's either empty or it contains a word
    FileRecord() {}

    FileRecord(std::string s)
    {
        this->_records.push_back(s);
    }

    FileRecord(char s[])
    {
        int len = strlen(s);
        this->_records.push_back(std::string(s, len));
    }

    FileRecord(std::vector<std::string> v)
    {
        this->_records = v;
    }

    // Returns the record number
    long write(std::fstream& outs)
    {
        outs.seekg(0, outs.end);
        long pos = outs.tellp();
        int size = this->_records.size();
        for (int i = 0; i < size; ++i) outs.write(this->_records[i].c_str(), MAX);
        return pos / (MAX * size);
    }
    // Returns the number of bytes read = MAX, or zero if read passed the end of file
    long read(std::fstream& ins, long recno)
    {
        int size = this->_records.size();
        long pos = recno * MAX * size;
        ins.seekg(pos);
        long total = 0;
        for (int i = 0; i < size; ++i)
        {
            char temp[MAX + 1];
            ins.read(temp, MAX);
            temp[ins.gcount()] = '\0';
            if (ins.gcount() == 0) return 0;
            this->_records[i] = temp;
            total += ins.gcount();
        }
        return total;
    }
    // Returns the record
    std::vector<std::string> get_records()
    {
        return this->_records;
    }
    // Overload the << operator to print a FileRecord
    friend std::ostream& operator<<(std::ostream& outs, const FileRecord& r)
    {
        for (std::string _record : r._records)
        {
            outs << std::setw(MAX / 4) << std::right << _record;
        }
        return outs;
    }
    void resize(int size) { this->_records.resize(size); }
};

#endif