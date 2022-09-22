#pragma once

#include <memory>
#include <vector>

#include "expressions.hpp"
#include "./common.hpp"
#include "../visitors/visitor.hpp"
#include "./token.hpp"

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
    TokenTypes type;

    LetStatement(std::unique_ptr<Expression> &expression, std::string name, TokenTypes type)
        : expression(std::move(expression)), name(name), type(type) {}

    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value *)
};

class BlockStatement : public Statement
{
public:
    std::vector<std::unique_ptr<Statement>> statements;
    BlockStatement(std::vector<std::unique_ptr<Statement>> &statements)
        : statements(std::move(statements)) {}

    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value *)
};

class IfStatement : public Statement
{
public:
    std::unique_ptr<Expression> condition;
    std::unique_ptr<Statement> thenBranch;
    std::unique_ptr<Statement> elseBranch;

    IfStatement(std::unique_ptr<Expression> &condition, std::unique_ptr<Statement> &thenBranch, std::unique_ptr<Statement> &elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value *)
};