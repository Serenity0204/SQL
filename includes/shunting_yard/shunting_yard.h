#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H
#pragma once

#include <cassert>
#include <iostream>

#include "../linked_list/queue/MyQueue.h"
#include "../linked_list/stack/MyStack.h"

#include "../token/token_includes.h"

class ShuntingYard
{
private:
    Queue<Token*> _queue;
    bool _error;

public:
    // CTOR
    ShuntingYard();
    ShuntingYard(const Queue<Token*>& input_q);

    // save input_q to member variable queue
    void infix(const Queue<Token*>& input_q);

    // generate postfix queue from infix queue
    Queue<Token*> postfix();
    Queue<Token*> postfix(const Queue<Token*>& input_q);
    // called by postfix()
    Queue<Token*> shunting_yard();
    bool is_error() { return this->_error; }
};

#endif // SHUNTING_YARD_H