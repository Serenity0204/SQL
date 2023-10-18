#ifndef RIGHTPAREN_H
#define RIGHTPAREN_H
#pragma once
#include "token.h"

class RightParen : public Token
{
private:
    std::string _str;

public:
    RightParen()
    {
        this->_str = ")";
    }
    ~RightParen() {}
    TOKEN_TYPE_PTR token_type() const override { return TOKEN_RIGHTPAREN; }
    std::string token_string() const override { return this->_str; }
    void print(std::ostream& outs) const override { outs << this->_str; }
    int precedence() const override { return -1; }
    friend std::ostream& operator<<(std::ostream& outs, const RightParen& token)
    {
        token.print(outs);
        return outs;
    }
    friend std::ostream& operator<<(std::ostream& outs, const RightParen* token)
    {
        token->print(outs);
        return outs;
    }
};

#endif