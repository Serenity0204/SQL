#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H

#include "../linked_list/queue/MyQueue.h"
#include "../linked_list/stack/MyStack.h"
#include <cassert>
#include <iostream>
#include <memory>

#include "../token/token_includes.h"

using namespace std;

class ShuntingYard
{
private:
    Queue<shared_ptr<Token>> _queue;
    bool _error;

public:
    // CTOR
    ShuntingYard();
    ShuntingYard(const Queue<shared_ptr<Token>>& input_q);

    // save input_q to member variable queue
    void infix(const Queue<shared_ptr<Token>>& input_q);

    // generate postfix queue from infix queue
    Queue<shared_ptr<Token>> postfix();
    Queue<shared_ptr<Token>> postfix(const Queue<shared_ptr<Token>>& input_q);
    // called by postfix()
    Queue<shared_ptr<Token>> shunting_yard();
    bool is_error() { return this->_error; }
};

#endif // SHUNTING_YARD_H