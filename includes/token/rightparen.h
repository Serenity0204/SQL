#ifndef RIGHTPAREN_H
#define RIGHTPAREN_H
#include "token.h"

class RightParen : public Token
{
private:
    string _str;

public:
    RightParen()
    {
        this->_str = ")";
    }
    ~RightParen() {}
    TOKEN_TYPE_PTR token_type() const override { return TOKEN_RIGHTPAREN; }
    virtual string token_string() const override { return this->_str; }
    virtual void print(ostream& outs) const override { outs << this->_str; }
    virtual int precedence() const { return -1; }
    friend ostream& operator<<(ostream& outs, const RightParen& token)
    {
        token.print(outs);
        return outs;
    }
    friend ostream& operator<<(ostream& outs, const RightParen* token)
    {
        token->print(outs);
        return outs;
    }
};

#endif