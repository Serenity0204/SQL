#ifndef HELPER_H
#define HELPER_H
#pragma once

#include "../linked_list/queue/MyQueue.h"
#include "../token/token_includes.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace Helper
{
    template <class T>
    inline bool is_in(const std::vector<T>& vec, const T& target)
    {
        for (int i = 0; i < vec.size(); ++i)
            if (vec[i] == target) return true;
        return false;
    }

    inline bool comparePair(const std::pair<std::string, long>& a, const std::pair<std::string, long>& b)
    {
        return a.second < b.second;
    }
    inline void obj_sort(std::vector<std::string>& vec1, std::vector<long>& vec2)
    {
        if (vec1.size() != vec2.size()) return;

        std::vector<std::pair<std::string, long>> to_sort;
        for (int i = 0; i < vec1.size(); ++i)
        {
            to_sort.push_back(std::make_pair(vec1[i], vec2[i]));
        }
        std::sort(to_sort.begin(), to_sort.end(), comparePair);

        vec1.clear();
        vec2.clear();
        for (auto p : to_sort)
        {
            vec1.push_back(p.first);
            vec2.push_back(p.second);
        }
    }
    inline std::vector<long> set_union(std::vector<long>& v1, std::vector<long>& v2)
    {
        std::vector<long> res;
        std::set<long> s;
        for (auto i : v1) s.insert(i);
        for (auto i : v2) s.insert(i);
        for (auto i : s) res.push_back(i);
        return res;
    }
    inline std::vector<long> set_intersection(std::vector<long>& v1, std::vector<long>& v2)
    {
        std::vector<long> res;
        std::map<long, int> m;
        for (auto i : v1)
        {
            if (!m.count(i))
            {
                m[i] = 1;
                continue;
            }
            m[i]++;
        }
        for (auto i : v2)
        {
            if (!m.count(i))
            {
                m[i] = 1;
                continue;
            }
            m[i]++;
        }
        for (auto pair : m)
            if (pair.second > 1) res.push_back(pair.first);

        return res;
    }
    inline void generate_tokens(const std::vector<std::string>& input, Queue<Token*>& infix)
    {
        for (const std::string& tk : input)
        {
            if (tk == ">=" || tk == "<=" || tk == ">" || tk == "<" || tk == "=" || tk == "!=" || tk == "LIKE")
            {
                infix.push(new Relational(tk));
                continue;
            }
            if (tk == "(")
            {
                infix.push(new LeftParen());
                continue;
            }
            if (tk == ")")
            {
                infix.push(new RightParen());
                continue;
            }
            if (tk == "and" || tk == "or")
            {
                infix.push(new Logical(tk));
                continue;
            }
            infix.push(new TokenStr(tk));
        }
    }
}
#endif // HELPER_H