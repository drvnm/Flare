#pragma once
#include <vector>
#include <memory>

#include "visitor.hpp"


class Compiler : public BaseVisitor
{
    int visit(LiteralExpression<int>&expression) override;
    int visit(BinaryExpression& expression) override;
    int visit(ExpressionStatement& atement) override;

public:
    int compile(std::vector<std::unique_ptr<Statement>>& statements);


};
