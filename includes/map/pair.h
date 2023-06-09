#ifndef PAIR_H
#define PAIR_H
#include <iostream>
using namespace std;

template <typename K, typename V>
struct Pair
{
    K key;
    V value;

    Pair(const K& k = K(), const V& v = V())
        : key(k), value(v)
    {
    }
    friend std::ostream& operator<<(std::ostream& outs, const Pair<K, V>& print_me)
    {
        outs << print_me.key << ":" << print_me.value << endl;
        return outs;
    }
    friend bool operator==(const Pair<K, V>& lhs, const Pair<K, V>& rhs) { return lhs.key == rhs.key; }
    friend bool operator<(const Pair<K, V>& lhs, const Pair<K, V>& rhs) { return lhs.key < rhs.key; }
    friend bool operator>(const Pair<K, V>& lhs, const Pair<K, V>& rhs) { return lhs.key > rhs.key; }
    friend bool operator<=(const Pair<K, V>& lhs, const Pair<K, V>& rhs) { return lhs.key <= rhs.key; }
    Pair& operator=(const Pair& source)
    {
        if (this == &source) return *this;
        this->key = source.key;
        this->value = source.value;
        return *this;
    }
};

#endif // PAIR_H