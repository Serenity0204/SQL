#ifndef RELATIONAL_H
#define RELATIONAL_H
#include "token.h"

class Relational : public Token
{
private:
    string _str;

public:
    Relational()
    {
        this->_str = "";
    }
    Relational(const string& input)
    {
        this->_str = input;
    }
    ~Relational() {}
    TOKEN_TYPE_PTR token_type() const override { return TOKEN_RELATIONAL; }
    virtual string token_string() const override { return this->_str; }
    int precedence() const override
    {
        if (this->_str == "<") return 4;
        if (this->_str == ">") return 4;
        if (this->_str == "<=") return 4;
        if (this->_str == ">=") return 4;
        if (this->_str == "=") return 4;
        if (this->_str == "!=") return 4;
        return -1;
    }
    virtual void print(ostream& outs) const override { outs << this->_str; }
    friend ostream& operator<<(ostream& outs, const Relational& token)
    {
        token.print(outs);
        return outs;
    }
    friend ostream& operator<<(ostream& outs, const Relational* token)
    {
        token->print(outs);
        return outs;
    }
};

#endif