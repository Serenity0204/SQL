#ifndef LOGICAL_H
#define LOGICAL_H
#include "token.h"

class Logical : public Token
{
private:
    string _str;

public:
    Logical()
    {
        this->_str = "";
    }
    Logical(const string& input)
    {
        this->_str = input;
    }
    ~Logical() {}
    TOKEN_TYPE_PTR token_type() const override { return TOKEN_LOGICAL; }
    virtual string token_string() const override { return this->_str; }
    virtual int precedence() const override
    {
        if (this->_str == "not") return 3;
        if (this->_str == "and") return 2;
        if (this->_str == "or") return 1;
        return -1;
    }
    virtual void print(ostream& outs) const override { outs << this->_str; }
    friend ostream& operator<<(ostream& outs, const Logical& token)
    {
        token.print(outs);
        return outs;
    }
    friend ostream& operator<<(ostream& outs, const Logical* token)
    {
        token->print(outs);
        return outs;
    }
};

#endif