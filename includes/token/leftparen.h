#ifndef LEFTPAREN_H
#define LEFTPAREN_H
#include "token.h"

class LeftParen : public Token
{
private:
    std::string _str;

public:
    LeftParen()
    {
        this->_str = "(";
    }
    ~LeftParen() {}
    TOKEN_TYPE_PTR token_type() const override { return TOKEN_LEFTPAREN; }
    std::string token_string() const override { return this->_str; }
    void print(std::ostream& outs) const override { outs << this->_str; }
    int precedence() const override { return -1; }
    friend std::ostream& operator<<(std::ostream& outs, const LeftParen& token)
    {
        token.print(outs);

        return outs;
    }
    friend std::ostream& operator<<(std::ostream& outs, const LeftParen* token)
    {
        token->print(outs);
        return outs;
    }
};

#endif