#ifndef LEFTPAREN_H
#define LEFTPAREN_H
#include "token.h"

class LeftParen : public Token
{
private:
    string _str;

public:
    LeftParen()
    {
        this->_str = "(";
    }
    ~LeftParen() {}
    TOKEN_TYPE_PTR token_type() const override { return TOKEN_LEFTPAREN; }
    virtual string token_string() const override { return this->_str; }
    virtual void print(ostream& outs) const override { outs << this->_str; }
    virtual int precedence() const { return -1; }
    friend ostream& operator<<(ostream& outs, const LeftParen& token)
    {
        token.print(outs);

        return outs;
    }
    friend ostream& operator<<(ostream& outs, const LeftParen* token)
    {
        token->print(outs);
        return outs;
    }
};

#endif