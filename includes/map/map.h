#ifndef MAP_H
#define MAP_H

#include "bplustree.h"
#include "pair.h"
#include <cstdlib> // Provides std::size_t
#include <vector>

template <typename K, typename V>
class Map
{
public:
    // TYPEDEFS and MEMBER CONSTANTS
    typedef BPlusTree<Pair<K, V>> map_base;
    // NESTED CLASS DEFINITION
    class Iterator
    {
    public:
        friend class Map;
        // CONSTRUCTORS
        Iterator(typename map_base::Iterator it = typename map_base::Iterator())
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
        Pair<K, V> operator*()
        {
            return *this->_it;
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
        typename map_base::Iterator _it;
    };

    // CONSTRUCTORS
    Map();
    Map& operator=(const Map& source)
    {
        if (this == &source) return *this;
        this->map = source.map;
        return *this;
    }
    // ITERATORS
    Iterator begin();
    Iterator end();
    // CAPACITY
    std::size_t size();
    std::size_t count() const;
    bool empty() const;
    // ELEMENT ACCESS
    V& operator[](const K& key);
    const V& operator[](const K& key) const;
    V& at(const K& key);
    const V& at(const K& key) const;
    // MODIFIERS
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    // OPERATIONS
    Iterator find(const K& key);
    bool contains(const Pair<K, V>& target) const;
    bool contains_key(const K& key);
    const V& get(const K& key) const;
    V& get(const K& key);
    Iterator lower_bound(const K& key);
    Iterator upper_bound(const K& key);
    bool is_valid() { return map.is_valid(); }
    // OVERLOADED OPERATORS
    friend std::ostream& operator<<(std::ostream& outs, const Map<K, V>& print_me)
    {
        outs << print_me.map << std::endl;
        return outs;
    }
    // getters
    std::vector<K> get_all_keys()
    {
        std::vector<K> keys;
        keys.clear();
        if (this->empty()) return keys;
        typename Map<K, V>::Iterator it;
        for (it = this->begin(); it != this->end(); ++it)
        {
            K key = (*it).key;
            keys.push_back(key);
        }
        return keys;
    }
    std::vector<V> get_all_values()
    {
        std::vector<V> vals;
        typename Map<K, V>::Iterator it;
        for (it = this->begin(); it != this->end(); ++it)
        {
            V v = (*it).value;
            vals += v;
        }
        return vals;
    }

private:
    BPlusTree<Pair<K, V>> map;
};

// CONSTRUCTORS
template <typename K, typename V>
Map<K, V>::Map()
    : map(BPlusTree<Pair<K, V>>())
{
}

// ITERATORS
template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::begin()
{
    return Iterator(this->map.begin());
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::end()
{
    return Iterator(this->map.end());
}
// CAPACITY

template <typename K, typename V>
std::size_t Map<K, V>::size()
{
    return this->map.size();
}

template <typename K, typename V>
std::size_t Map<K, V>::count() const
{
    return this->map.count();
}

template <typename K, typename V>
bool Map<K, V>::empty() const
{
    return this->map.empty();
}

// ELEMENT ACCESS

template <typename K, typename V>
V& Map<K, V>::operator[](const K& key)
{
    return this->at(key);
}

template <typename K, typename V>
const V& Map<K, V>::operator[](const K& key) const
{
    return this->at(key);
}

template <typename K, typename V>
V& Map<K, V>::at(const K& key)
{
    return this->map.get(key).value;
}

template <typename K, typename V>
const V& Map<K, V>::at(const K& key) const
{
    return this->map.get(key).value;
}

// MODIFIERS

template <typename K, typename V>
void Map<K, V>::insert(const K& k, const V& v)
{
    this->map.insert(Pair<K, V>(k, v));
}

template <typename K, typename V>
void Map<K, V>::erase(const K& key)
{
    this->map.erase(Pair<K, V>(key));
}

template <typename K, typename V>
void Map<K, V>::clear()
{
    this->map.clear_tree();
}
// OPERATIONS

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::find(const K& key)
{
    return Iterator(this->map.find(Pair<K, V>(key)));
}

template <typename K, typename V>
bool Map<K, V>::contains(const Pair<K, V>& target) const
{
    return this->map.contains(target);
}

template <typename K, typename V>
bool Map<K, V>::contains_key(const K& key)
{
    return this->map.contains(Pair<K, V>(key));
}

template <typename K, typename V>
const V& Map<K, V>::get(const K& key) const
{
    return this->at(key);
}

template <typename K, typename V>
V& Map<K, V>::get(const K& key)
{
    return this->at(key);
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::lower_bound(const K& key)
{
    return Iterator(this->map.lower_bound(Pair<K, V>(key)));
}

template <typename K, typename V>
typename Map<K, V>::Iterator Map<K, V>::upper_bound(const K& key)
{
    return Iterator(this->map.upper_bound(Pair<K, V>(key)));
}
#endif // MAP_H