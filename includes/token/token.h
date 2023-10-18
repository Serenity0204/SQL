#ifndef TOKEN_H
#define TOKEN_H
#include "constants.h"

class Token;

class Token
{
public:
    Token() {}
    ~Token() {}
    virtual TOKEN_TYPE_PTR token_type() const = 0;
    virtual std::string token_string() const = 0;
    virtual void print(std::ostream& outs) const = 0;
    virtual int precedence() const = 0;
    friend std::ostream& operator<<(std::ostream& outs, const Token& token)
    {
        token.print(outs);
        return outs;
    }
    friend std::ostream& operator<<(std::ostream& outs, const Token* token)
    {
        token->print(outs);
        return outs;
    }
};

#endif