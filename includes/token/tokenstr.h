#ifndef TOKENSTR_H
#define TOKENSTR_H
#include "token.h"

class TokenStr : public Token
{
private:
    std::string _str;

public:
    TokenStr()
    {
        this->_str = "";
    }
    TokenStr(const std::string& input)
    {
        this->_str = input;
    }
    ~TokenStr() {}
    TOKEN_TYPE_PTR token_type() const override { return TOKEN_TOKENSTR; }
    std::string token_string() const override { return this->_str; }
    void print(std::ostream& outs) const override { outs << this->_str; }
    int precedence() const override { return -1; }
    friend std::ostream& operator<<(std::ostream& outs, const TokenStr& token)
    {
        token.print(outs);
        return outs;
    }
    friend std::ostream& operator<<(std::ostream& outs, const TokenStr* token)
    {
        token->print(outs);
        return outs;
    }
};

#endif