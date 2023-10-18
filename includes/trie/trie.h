#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

struct TrieNode
{
public:
    std::unordered_map<char, std::shared_ptr<TrieNode>> _children;
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
        this->_root = std::make_shared<TrieNode>();
    }

    void insert(std::string word)
    {
        std::shared_ptr<TrieNode> walker = this->_root;
        for (char c : word)
        {
            if (!walker->_children.count(c)) walker->_children[c] = std::make_shared<TrieNode>();
            walker = walker->_children[c];
        }
        walker->_is_word = true;
    }

    std::vector<std::string> get_prefix(std::string prefix)
    {
        std::vector<std::string> res;
        std::shared_ptr<TrieNode> walker = _root;
        for (char c : prefix)
        {
            if (!walker->_children.count(c)) return res;
            walker = walker->_children[c];
        }
        this->_dfs(walker, prefix, res);
        return res;
    }

private:
    std::shared_ptr<TrieNode> _root;

    void _dfs(std::shared_ptr<TrieNode> node, std::string cur_word, std::vector<std::string>& res)
    {
        if (node->_is_word) res.push_back(cur_word);
        for (auto& pair : node->_children) this->_dfs(pair.second, cur_word + pair.first, res);
    }
};

#endif