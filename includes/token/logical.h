#ifndef LOGICAL_H
#define LOGICAL_H
#pragma once

#include "token.h"

class Logical : public Token
{
private:
    std::string _str;

public:
    Logical()
    {
        this->_str = "";
    }
    Logical(const std::string& input)
    {
        this->_str = input;
    }
    ~Logical() {}
    TOKEN_TYPE_PTR token_type() const override { return TOKEN_LOGICAL; }
    std::string token_string() const override { return this->_str; }
    int precedence() const override
    {
        if (this->_str == "not") return 3;
        if (this->_str == "and") return 2;
        if (this->_str == "or") return 1;
        return -1;
    }
    void print(std::ostream& outs) const override { outs << this->_str; }
    friend std::ostream& operator<<(std::ostream& outs, const Logical& token)
    {
        token.print(outs);
        return outs;
    }
    friend std::ostream& operator<<(std::ostream& outs, const Logical* token)
    {
        token->print(outs);
        return outs;
    }
};

#endif