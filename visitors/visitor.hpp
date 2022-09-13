
#pragma once


template <typename T>
class LiteralExpression;
class BinaryExpression;
class Statement;
class ExpressionStatement;

// forward declare all the expressions and statements

class BaseVisitor {
public:
    virtual int visit(LiteralExpression<int> expression) = 0;
    virtual int visit(BinaryExpression expression) = 0;
    virtual int visit(ExpressionStatement statement) = 0;
};


