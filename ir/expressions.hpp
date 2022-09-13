#pragma once

#include "token.hpp"
#include "../visitors/visitor.hpp"

class Expression
{
public:
    virtual int accept(BaseVisitor &visitor) {
        return 0;
    }
};

template <typename T>
class LiteralExpression : public Expression
{
public:
    T value;
    LiteralExpression(T value) : value(value) {}
    int accept(BaseVisitor &visitor) override
    {
        return visitor.visit(*this);
    }
};

class BinaryExpression : public Expression
{
public:
    Expression left;    
    Expression right;
    Token op;
    BinaryExpression(Expression left, Token op, Expression right) : left(left), right(right), op(op) {}

    int accept(BaseVisitor &visitor) override;
};
