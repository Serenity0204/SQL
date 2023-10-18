#ifndef LINKED_LIST_FUNCTIONS_H
#define LINKED_LIST_FUNCTIONS_H
#pragma once

#include "../node/node.h"
#include <cassert>

// Declaration
// Linked List General Functions:
template <typename T>
void _print_list(Node<T>* head);

// recursive fun! :)
template <typename T>
void _print_list_backwards(Node<T>* head);

// return ptr to key or NULL
template <typename T>
Node<T>* _search_list(Node<T>* head, T key);

template <typename T>
Node<T>* _insert_head(Node<T>*& head, T insert_this);

// insert after ptr
template <typename T>
Node<T>* _insert_after(Node<T>*& head, Node<T>* after_this, T insert_this);

// insert before ptr
template <typename T>
Node<T>* _insert_before(Node<T>*& head, Node<T>* before_this, T insert_this);

// ptr to previous node
template <typename T>
Node<T>* _previous_node(Node<T>* head, Node<T>* prev_to_this);

// delete, return item
template <typename T>
T _delete_node(Node<T>*& head, Node<T>* delete_this);

// duplicate the list
template <typename T>
Node<T>* _copy_list(Node<T>* head);

// duplicate list and return the last node of the copy
template <typename T>
Node<T>* _copy_list(Node<T>*& dest, Node<T>* src);

// delete all the nodes
template <typename T>
void _clear_list(Node<T>*& head);

//_item at this position
template <typename T>
T& _at(Node<T>* head, int pos);

template <typename T>
Node<T>* _insert_tail(Node<T>*& head, T insert_this);

// Last node in the list, never use this function.
template <typename T>
Node<T>* _last_node(Node<T>* head);

// Definition

// TODO

// Linked List General Functions:
template <typename T>
void _print_list(Node<T>* head)
{
    Node<T>* walker = head;
    while (walker != nullptr)
    {
        std::cout << *walker;
        walker = walker->_next;
    }
    std::cout << "|||" << std::endl;
}

// recursive fun! :)
template <typename T>
void _print_list_backwards(Node<T>* head)
{
    if (head == nullptr)
    {
        std::cout << "|||";
        return;
    }

    _print_list_backwards(head->_next);
    std::cout << *head;
}

// return ptr to key or NULL
template <typename T>
Node<T>* _search_list(Node<T>* head, T key)
{
    if (head == nullptr || head->_item == key) return head;
    return _search_list<T>(head->_next, key);
}

template <typename T>
Node<T>* _insert_head(Node<T>*& head, T insert_this)
{
    Node<T>* newNode = new Node<T>(insert_this);
    if (head == nullptr)
    {
        head = newNode;
        return head;
    }

    newNode->_next = head;
    head->_prev = newNode;
    head = newNode;
    return head;
}

template <typename T>
Node<T>* _insert_tail(Node<T>*& head, T insert_this)
{
    Node<T>* lastNode = _last_node(head);
    if (lastNode == nullptr)
    {
        head = _insert_head<T>(head, insert_this);
        return head;
    }
    Node<T>* newNode = new Node<T>(insert_this);
    lastNode->_next = newNode;
    newNode->_prev = lastNode;
    return newNode;
}

// insert after ptr
template <typename T>
Node<T>* _insert_after(Node<T>*& head, Node<T>* after_this, T insert_this)
{
    if (head == nullptr)
    {
        head = _insert_head<T>(head, insert_this);
        return head;
    }
    if ((head->_next == nullptr && head == after_this) || after_this == _last_node<T>(head))
    {
        Node<T>* lastNode = _insert_tail<T>(head, insert_this);
        return lastNode;
    }

    Node<T>* newNode = new Node<T>(insert_this);
    Node<T>* nextNode = after_this->_next;
    newNode->_next = nextNode;
    nextNode->_prev = newNode;
    after_this->_next = newNode;
    newNode->_prev = after_this;
    return newNode;
}

// insert before ptr
template <typename T>
Node<T>* _insert_before(Node<T>*& head, Node<T>* before_this, T insert_this)
{
    if (head == nullptr || head == before_this)
    {
        head = _insert_head<T>(head, insert_this);
        return head;
    }

    Node<T>* newNode = new Node<T>(insert_this);
    Node<T>* prevNode = before_this->_prev;
    Node<T>* nextNode = before_this;
    prevNode->_next = newNode;
    newNode->_prev = prevNode;
    newNode->_next = nextNode;
    nextNode->_prev = newNode;
    return newNode;
}

// ptr to previous node
template <typename T>
Node<T>* _previous_node(Node<T>* head, Node<T>* prev_to_this)
{
    if (prev_to_this == head) return nullptr;
    return prev_to_this->_prev;
}

// delete, return item
template <typename T>
T _delete_node(Node<T>*& head, Node<T>* delete_this)
{
    T val = delete_this->_item;

    if (head == delete_this)
    {
        Node<T>* newHead = head->_next;
        head = newHead;
        delete delete_this;
        return val;
    }

    Node<T>* lastNode = _last_node(head);
    if (delete_this == lastNode)
    {
        Node<T>* prevNode = lastNode->_prev;
        delete delete_this;
        return val;
    }

    Node<T>* prevNode = delete_this->_prev;
    Node<T>* nextNode = delete_this->_next;
    prevNode->_next = nextNode;
    nextNode->_prev = prevNode;

    delete delete_this;
    return val;
}

// duplicate the list
template <typename T>
Node<T>* _copy_list(Node<T>* head)
{
    if (head == nullptr) return nullptr;

    Node<T>* headWalker = head;                  // w1
    Node<T>* newNode = new Node<T>(head->_item); // w2
    Node<T>* newHeadHold = newNode;              // hold
    while (headWalker->_next != nullptr)
    {
        Node<T>* nextNode = new Node<T>(headWalker->_next->_item);
        newNode->_next = nextNode;
        nextNode->_prev = newNode;
        headWalker = headWalker->_next;
        newNode = newNode->_next;
    }
    return newHeadHold;
}

// duplicate list and return the last node of the copy
template <typename T>
Node<T>* _copy_list(Node<T>*& dest, Node<T>* src)
{
    if (dest != nullptr) _clear_list(dest);
    dest = _copy_list<T>(src);
    return _last_node<T>(dest);
}

// delete all the nodes
template <typename T>
void _clear_list(Node<T>*& head)
{
    if (head == nullptr) return;
    Node<T>* toDelete = head;
    head = head->_next;
    delete toDelete;
    _clear_list(head);
}

//_item at this position
template <typename T>
T& _at(Node<T>* head, int pos)
{
    if (pos == 0) return head->_item;
    pos--;
    return _at<T>(head->_next, pos);
}

// Last node in the list, never use this function.
template <typename T>
Node<T>* _last_node(Node<T>* head)
{
    if (head == nullptr) return head;
    if (head != nullptr && head->_next == nullptr) return head;
    return _last_node<T>(head->_next);
}

#endif // LINKED_LIST_FUNCTIONS_H