#pragma once

#include <memory>

#include "token.hpp"
#include "../visitors/visitor.hpp"

class Expression
{
public:
    virtual int accept(BaseVisitor &visitor)
    {
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
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    Token op;
    BinaryExpression(
        std::unique_ptr<Expression>& left,
        Token op,
        std::unique_ptr<Expression>& right)
        : left(std::move(left)),
          right(std::move(right)),
          op(op) {}

    int accept(BaseVisitor &visitor) override;
};
