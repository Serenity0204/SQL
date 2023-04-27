#ifndef TOKENSTR_H
#define TOKENSTR_H
#include "token.h"

class TokenStr : public Token
{
private:
    string _str;

public:
    TokenStr()
    {
        this->_str = "";
    }
    TokenStr(const string& input)
    {
        this->_str = input;
    }
    ~TokenStr() {}
    TOKEN_TYPE_PTR token_type() const override { return TOKEN_TOKENSTR; }
    virtual string token_string() const override { return this->_str; }
    virtual void print(ostream& outs) const override { outs << this->_str; }
    virtual int precedence() const { return -1; }
    friend ostream& operator<<(ostream& outs, const TokenStr& token)
    {
        token.print(outs);
        return outs;
    }
    friend ostream& operator<<(ostream& outs, const TokenStr* token)
    {
        token->print(outs);
        return outs;
    }
};

#endif