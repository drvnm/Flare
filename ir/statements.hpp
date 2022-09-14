#pragma once

#include <memory>

#include "expressions.hpp"
#include "../visitors/visitor.hpp"

class Statement
{
public:
    virtual int accept(BaseVisitor &visitor)
    {
        return 0;
    }
};

class ExpressionStatement : public Statement
{
public:
    std::unique_ptr<Expression> expression;
    ExpressionStatement(std::unique_ptr<Expression>& expression)
        : expression(std::move(expression)) {}

    int accept(BaseVisitor &visitor) override;
};
