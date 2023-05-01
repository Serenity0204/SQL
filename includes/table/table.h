#ifndef TABLE_H
#define TABLE_H

#include <cassert>  // assert
#include <cstring>  // strcmp
#include <fstream>  // fstream
#include <iomanip>  // setw
#include <iostream> // cout, endl
#include <set>      // std::set
#include <string>   // string
#include <vector>   // vector

#include "../binary_files/file_record.h"
#include "../binary_files/utilities.h"
#include "../helper/helper.h"
#include "../linked_list/queue/MyQueue.h"
#include "../map/map.h"
#include "../map/mmap.h"
#include "../shunting_yard/shunting_yard.h"
#include "../token/token_includes.h"
#include "../trie/trie.h"

class Table
{
private:
    // iterator typedef
    typedef MMap<string, long>::Iterator mmap_sl_it;
    // title of the table
    string _table_name;
    // fields of the table
    vector<string> _field_names;
    vector<string> _selected_field_names;
    // number of records in the table
    long _n_records;
    // selected records' numbers
    vector<long> _record_indices;
    long _total_records;
    // Map<field_name->string, MMap<field_value->string, index->long>>
    // field name: name age gender
    //             John 15  Male -> first entry
    //             David 19 Male -> second entry
    // reason to make map of mmap is that multiple entries could have the same multiple value
    // cache:
    // name: {John: 1, David: 2};
    // age: {15: 1, 19: 2};
    // gender {Male: 1, 2}
    // first and second entries have the same gender, so we gotta map gender-> Male -> 1st, 2nd entry

    Map<string, MMap<string, long>> _cache;
    Map<string, long> _field_name_indices; // map field name to entry indices

    vector<string> _to_print;
    // helper
    void _init(bool is_new);
    void _index();
    vector<long> _select_helper(const string& field_name, const string& op, const string& field_value);
    void _read_helper(Table& temp);
    vector<long> _rpn(const Queue<Token*>& postfix);

    inline vector<long> _bound_helper(const mmap_sl_it& begin, const mmap_sl_it& end)
    {
        vector<long> res;
        res.clear();
        mmap_sl_it it = begin;
        if (it.is_null()) return res;
        // *it gives you Mpair
        for (it = begin; it != end; ++it) res += (*it).value_list;

        return res;
    }

    inline void _reorder_fields(const vector<string>& selected_fields, vector<string>& reordered_vec)
    {
        vector<string> reordered = selected_fields;
        // search for invalids
        vector<int> invalid_indices;
        for (int i = 0; i < reordered.size(); ++i)
        {
            string field_name = reordered[i];
            if (!this->_field_name_indices.contains(field_name)) invalid_indices.push_back(i);
        }
        // erase the invalid indices
        for (auto invalid_i : invalid_indices)
        {
            vector<string>::iterator it = reordered.begin();
            reordered.erase(it + invalid_i);
        }

        vector<long> sort_indices;
        // sort
        for (auto field_name : reordered)
        {
            long idx = this->_field_name_indices[field_name];
            sort_indices += idx;
        }
        Helper::obj_sort(reordered, sort_indices);
        reordered_vec = reordered;
    }
    inline bool _check_error_fields(const vector<string>& selected_fields)
    {
        set<string> s;
        // check duplicate
        for (auto field : selected_fields)
        {
            if (s.count(field)) return false;

            s.insert(field);
        }
        return true;
    }

public:
    // TYPEDEFS
    typedef vector<string> vector_str;
    typedef vector<long> vector_long;
    typedef Map<string, long> map_sl;
    typedef Map<string, string> map_ss;
    typedef MMap<string, long> mmap_sl;
    typedef MMap<string, string> mmap_ss;
    // CONSTRUCTORS
    Table();
    Table(const string& table_name);
    Table(const string& table_name, const vector<string>& field_names);
    Table& operator=(const Table& RHS);
    ~Table() {}

    // SQL: CREATE TABLE
    string create_table(const string& table_name, const vector<string>& field_names);

    // SQL: INSERT INTO
    // string insert_into(const vector<char*>& field_values); ??
    string insert_into(const vector<string>& field_values);

    // SQL: SELECT
    Table select(const vector<string>& selected_fields, const string& field_name, const string& op, const string& field_value);
    Table select(const vector<string>& selected_fields, const vector<string>& expression = vector<string>());
    Table select(const vector<string>& selected_fields, const Queue<Token*>& expression);
    Table select_all(const vector<string>& selected_fields = vector<string>());

    // Get all selected record numbers
    inline vector<long> select_recnos() const { return _record_indices; }
    // Print table
    friend ostream& operator<<(ostream& outs, const Table& t);
    // Get the title of the table
    inline string title() const { return _table_name; }
    // Get the fields of the table
    inline vector<string> get_fields() { return this->_selected_field_names; }
    inline vector<string> get_original_fields() { return this->_field_names; }
    // Get the number of records in the table
    inline long record_count() const { return this->_n_records; }
    inline long total_records() const { return this->_total_records; }
};

#endif // TABLE_H