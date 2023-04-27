#include "shunting_yard.h"

ShuntingYard::ShuntingYard()
{
    this->_queue = Queue<Token*>();
    this->_error = false;
}

ShuntingYard::ShuntingYard(const Queue<Token*>& input_q)
{
    this->_queue = input_q;
    this->_error = false;
}

void ShuntingYard::infix(const Queue<Token*>& input_q)
{
    this->_queue = input_q;
    this->_error = false;
}

// generate postfix queue from infix queue
Queue<Token*> ShuntingYard::postfix()
{
    return this->shunting_yard();
}

Queue<Token*> ShuntingYard::postfix(const Queue<Token*>& input_q)
{
    this->_queue = input_q;
    this->_error = false;
    return this->shunting_yard();
}

// called by postfix()
Queue<Token*> ShuntingYard::shunting_yard()
{
    Queue<Token*> output_queue;
    Stack<Token*> operator_stack;

    while (!this->_queue.empty())
    {
        Token* token = this->_queue.pop();
        if (token->token_type() == TOKEN_TOKENSTR)
        {
            output_queue.push(token);
            continue;
        }
        if (token->token_type() == TOKEN_LEFTPAREN)
        {
            operator_stack.push(token);
            continue;
        }
        if (token->token_type() == TOKEN_RELATIONAL)
        {
            operator_stack.push(token);
            continue;
        }
        if (token->token_type() == TOKEN_LOGICAL)
        {
            while (!operator_stack.empty() && operator_stack.top()->token_type() != TOKEN_LEFTPAREN && operator_stack.top()->precedence() >= token->precedence()) output_queue.push(operator_stack.pop());
            // output_queue.push(token);
            operator_stack.push(token);
            continue;
        }
        if (token->token_type() == TOKEN_RIGHTPAREN)
        {
            while (!operator_stack.empty() && operator_stack.top()->token_type() != TOKEN_LEFTPAREN)
            {
                output_queue.push(operator_stack.pop());
                if (operator_stack.empty())
                {
                    output_queue.clear();
                    this->_error = true;
                    return output_queue;
                }
            }

            if (operator_stack.empty() || operator_stack.top()->token_type() != TOKEN_LEFTPAREN)
            {
                output_queue.clear();
                this->_error = true;
                return output_queue;
            }
            operator_stack.pop();
        }
    }
    while (!operator_stack.empty())
    {
        if (operator_stack.top()->token_type() == TOKEN_LEFTPAREN)
        {
            output_queue.clear();
            this->_error = true;
            return output_queue;
        }
        output_queue.push(operator_stack.pop());
    }
    return output_queue;
}
