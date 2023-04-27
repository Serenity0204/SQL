#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <cstring>  // strncpy
#include <fstream>  // fstream
#include <iomanip>  // setw, right
#include <iostream> // cout, endl
#include <memory>
#include <vector> // vector
using namespace std;

class FileRecord
{

private:
    // The maximum size of the record
    static const int MAX = 100;
    // The record vector
    vector<string> _records;

public:
    // When construct a FileRecord, it's either empty or it contains a word
    FileRecord() {}

    FileRecord(string s)
    {
        this->_records.push_back(s);
    }

    FileRecord(char s[])
    {
        int len = strlen(s);
        this->_records.push_back(string(s, len));
    }

    FileRecord(vector<string> v)
    {
        this->_records = v;
    }

    // Returns the record number
    long write(fstream& outs)
    {
        outs.seekg(0, outs.end);
        long pos = outs.tellp();
        int size = this->_records.size();
        for (int i = 0; i < size; ++i) outs.write(this->_records[i].c_str(), MAX);
        return pos / (MAX * size);
    }
    // Returns the number of bytes read = MAX, or zero if read passed the end of file
    long read(fstream& ins, long recno)
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
    vector<char*> get_records()
    {
        vector<char*> res;
        for (int i = 0; i < this->_records.size(); ++i)
        {
            string record = this->_records[i];
            char* temp = new char[record.size() + 1];
            strncpy(temp, record.c_str(), record.size());
            temp[record.size()] = '\0';
            res.push_back(temp);
        }
        return res;
    }
    vector<string> get_records_string()
    {
        return this->_records;
    }
    // Overload the << operator to print a FileRecord
    friend ostream& operator<<(ostream& outs, const FileRecord& r)
    {
        for (string _record : r._records)
        {
            outs << setw(MAX / 4) << right << _record;
        }
        return outs;
    }
    void resize(int size) { this->_records.resize(size); }
};

#endif