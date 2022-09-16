#pragma once

#include <memory>

#include "expressions.hpp"
#include "./common.hpp"
#include "../visitors/visitor.hpp"

class Statement
{
public:
    virtual llvm::Value* accept(BaseVisitor &visitor)
    {
        return nullptr;
    }
};

class ExpressionStatement : public Statement
{
public:
    std::unique_ptr<Expression> expression;
    ExpressionStatement(std::unique_ptr<Expression> &expression)
        : expression(std::move(expression)) {}

    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value *)
};

class PrintStatement : public Statement
{
public:
    std::unique_ptr<Expression> expression;
    PrintStatement(std::unique_ptr<Expression> &expression)
        : expression(std::move(expression)) {}

    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value *)
};

class LetStatement : public Statement
{
public:
    std::unique_ptr<Expression> expression;
    std::string name;
    Token type;

    LetStatement(std::unique_ptr<Expression> &expression, std::string name, Token type)
        : expression(std::move(expression)), name(name), type(type) {}

    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value *)
};