#ifndef MMAP_H
#define MMAP_H
#pragma once

#include "bplustree.h"
#include "mpair.h"
#include <cstdlib> // Provides std::size_t
#include <set>

template <typename K, typename V>
class MMap
{
private:
    BPlusTree<MPair<K, V>> mmap;

public:
    // TYPEDEFS and MEMBER CONSTANTS
    typedef BPlusTree<MPair<K, V>> mmap_base;
    // NESTED CLASS DEFINITION
    class Iterator
    {
    public:
        friend class MMap;
        // CONSTRUCTORS
        Iterator(typename mmap_base::Iterator it = typename mmap_base::Iterator())
            : _it(it)
        {
        }

        // OPERATORS
        Iterator operator++(int unused)
        {
            Iterator hold = *this;
            this->_it++;
            return hold;
        }
        Iterator operator++()
        {
            ++this->_it;
            return *this;
        }
        MPair<K, V> operator*()
        {
            return *this->_it;
        }
        bool is_null()
        {
            return this->_it.is_null();
        }
        friend bool operator==(const Iterator& lhs, const Iterator& rhs)
        {
            return lhs._it == rhs._it;
        }
        friend bool operator!=(const Iterator& lhs, const Iterator& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        typename mmap_base::Iterator _it;
    };

    // CONSTRUCTORS
    MMap()
        : mmap(BPlusTree<MPair<K, V>>())
    {
    }
    MMap& operator=(const MMap& source)
    {
        if (this == &source) return *this;
        this->mmap = source.mmap;
        return *this;
    }
    // ITERATORS
    Iterator begin() { return Iterator(this->mmap.begin()); }
    Iterator end() { return Iterator(this->mmap.end()); }
    // CAPACITY
    std::size_t size() { return this->mmap.size(); }

    std::size_t size_list()
    {
        if (this->empty()) return 0;
        typename MMap<K, V>::Iterator it;
        size_t size = 0;
        for (it = this->begin(); it != this->end(); ++it) size += (*it).value_list.size();
        return size;
    }

    std::size_t count() const { return this->mmap.count(); }
    bool empty() const { return this->mmap.empty(); }

    // ELEMENT ACCESS
    std::vector<V>& at(const K& key) { return this->mmap.get(MPair<K, V>(key)).value_list; }
    const std::vector<V>& at(const K& key) const { return this->mmap.get(MPair<K, V>(key)).value_list; }
    const std::vector<V>& operator[](const K& key) const { return this->at(key); }
    std::vector<V>& operator[](const K& key) { return this->at(key); }

    // MODIFIERS
    void insert_key(const K& key)
    {
        assert(!this->contains(key));
        this->mmap.insert(MPair<K, V>(key));
    }
    void insert(const K& k, const V& v)
    {
        if (this->contains(k))
        {
            this->at(k).push_back(v);
            return;
        }
        this->mmap.insert(MPair<K, V>(k, v));
    }
    void erase(const K& key) { this->mmap.erase(MPair<K, V>(key)); }
    void clear() { this->mmap.clear_tree(); }

    // OPERATIONS
    Iterator find(const K& key) { return Iterator(this->mmap.find(MPair<K, V>(key))); }
    bool contains(const K& key) const { return this->mmap.contains(MPair<K, V>(key)); }
    std::vector<V>& get(const K& key) { return this->mmap.get(MPair<K, V>(key)).value_list; }
    const std::vector<V>& get(const K& key) const { return this->mmap.get(MPair<K, V>(key)).value_list; }

    Iterator lower_bound(const K& key) { return Iterator(this->mmap.lower_bound(MPair<K, V>(key))); }
    Iterator upper_bound(const K& key) { return Iterator(this->mmap.upper_bound(MPair<K, V>(key))); }
    bool is_valid() { return mmap.is_valid(); }
    // OVERLOADED OPERATORS
    friend std::ostream& operator<<(std::ostream& outs, const MMap<K, V>& print_me)
    {
        outs << print_me.mmap << std::endl;
        return outs;
    }
    // getters
    std::vector<K> keys()
    {
        std::vector<K> keys;
        keys.clear();
        if (this->empty()) return keys;

        typename MMap<K, V>::Iterator it;
        for (it = this->begin(); it != this->end(); ++it)
        {
            K key = (*it).key;
            keys.push_back(key);
        }
        return keys;
    }
    std::vector<V> values()
    {
        std::vector<V> values;
        values.clear();
        if (this->empty()) return values;

        typename MMap<K, V>::Iterator it;
        for (it = this->begin(); it != this->end(); ++it)
        {
            std::vector<V> vals = (*it).value_list;
            values += vals;
        }
        return values;
    }

    void print_lookup()
    {
        if (this->empty())
        {
            std::cout << "empty map" << std::endl;
            return;
        }
        typename MMap<K, V>::Iterator it;
        for (it = this->begin(); it != this->end(); ++it) std::cout << std::setw(10) << (*it).key << " :" << std::setw(5) << " " << (*it).value_list << std::endl;
    }
};

#endif // MMAP_H