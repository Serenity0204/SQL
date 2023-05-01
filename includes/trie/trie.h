#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

using namespace std;

struct TrieNode
{
public:
    unordered_map<char, shared_ptr<TrieNode>> _children;
    bool _is_word;
    TrieNode()
        : _is_word(false)
    {
    }
    ~TrieNode() {}
};

class Trie
{
public:
    Trie()
    {
        this->_root = make_shared<TrieNode>();
    }

    void insert(string word)
    {
        shared_ptr<TrieNode> walker = this->_root;
        for (char c : word)
        {
            if (!walker->_children.count(c)) walker->_children[c] = make_shared<TrieNode>();
            walker = walker->_children[c];
        }
        walker->_is_word = true;
    }

    vector<string> get_prefix(string prefix)
    {
        vector<string> res;
        shared_ptr<TrieNode> walker = _root;
        for (char c : prefix)
        {
            if (!walker->_children.count(c)) return res;
            walker = walker->_children[c];
        }
        this->_dfs(walker, prefix, res);
        return res;
    }

private:
    shared_ptr<TrieNode> _root;

    void _dfs(shared_ptr<TrieNode> node, string cur_word, vector<string>& res)
    {
        if (node->_is_word) res.push_back(cur_word);
        for (auto& pair : node->_children) this->_dfs(pair.second, cur_word + pair.first, res);
    }
};

#endif