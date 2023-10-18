#ifndef RELATIONAL_H
#define RELATIONAL_H
#pragma once
#include "token.h"

class Relational : public Token
{
private:
    std::string _str;

public:
    Relational()
    {
        this->_str = "";
    }
    Relational(const std::string& input)
    {
        this->_str = input;
    }
    ~Relational() {}
    TOKEN_TYPE_PTR token_type() const override { return TOKEN_RELATIONAL; }
    std::string token_string() const override { return this->_str; }
    int precedence() const override
    {
        if (this->_str == "<") return 4;
        if (this->_str == ">") return 4;
        if (this->_str == "<=") return 4;
        if (this->_str == ">=") return 4;
        if (this->_str == "=") return 4;
        if (this->_str == "!=") return 4;
        if (this->_str == "LIKE") return 4;
        return -1;
    }
    void print(std::ostream& outs) const override { outs << this->_str; }
    friend std::ostream& operator<<(std::ostream& outs, const Relational& token)
    {
        token.print(outs);
        return outs;
    }
    friend std::ostream& operator<<(std::ostream& outs, const Relational* token)
    {
        token->print(outs);
        return outs;
    }
};

#endif