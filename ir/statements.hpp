#pragma once

#include "expressions.hpp"
#include "../visitors/visitor.hpp"

class Statement {
public:
    virtual int accept(BaseVisitor& visitor) {
        return 0;
    }

};

class ExpressionStatement : public Statement {
public:
    Expression expression;
    ExpressionStatement(Expression expression) : expression(expression) {}

    int accept(BaseVisitor& visitor) override;
};
    
