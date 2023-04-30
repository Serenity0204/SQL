#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
using namespace std;

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>
using namespace std;

class Trie
{
public:
    Trie()
    {
        root = make_shared<TrieNode>();
    }

    void insert(string word)
    {
        shared_ptr<TrieNode> cur = root;
        for (char c : word)
        {
            if (!cur->children.count(c))
            {
                cur->children[c] = make_shared<TrieNode>();
            }
            cur = cur->children[c];
        }
        cur->is_word = true;
    }

    vector<string> get_prefix(string prefix)
    {
        vector<string> res;
        shared_ptr<TrieNode> cur = root;
        for (char c : prefix)
        {
            if (!cur->children.count(c))
            {
                return res;
            }
            cur = cur->children[c];
        }
        dfs(cur, prefix, res);
        return res;
    }

private:
    struct TrieNode
    {
        unordered_map<char, shared_ptr<TrieNode>> children;
        bool is_word = false;
    };

    shared_ptr<TrieNode> root;

    void dfs(shared_ptr<TrieNode> node, string cur_word, vector<string>& res)
    {
        if (node->is_word)
        {
            res.push_back(cur_word);
        }
        for (auto& p : node->children)
        {
            dfs(p.second, cur_word + p.first, res);
        }
    }
};

#endif