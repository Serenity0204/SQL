#ifndef NODE_H
#define NODE_H
#pragma once

#include <iostream>

template <typename T>
struct Node
{
public:
    Node(const T& item = T(), Node* next = nullptr);

    template <typename U>
    friend std::ostream& operator<<(std::ostream& outs, const Node<U>& printMe);

    T _item;
    Node* _next;
    Node* _prev;
};

// Definition

// TODO
template <typename T>
Node<T>::Node(const T& item, Node<T>* next)
{
    this->_item = item;
    this->_next = next;
    this->_prev = nullptr;
}

template <typename U>
std::ostream& operator<<(std::ostream& outs, const Node<U>& printMe)
{
    outs << "<-[" << printMe._item << "]->";
    return outs;
}
#endif // NODE_H