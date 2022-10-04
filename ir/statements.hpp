#pragma once

#include <memory>
#include <vector>
#include "llvm/IR/IRBuilder.h"


#include "expressions.hpp"
#include "./common.hpp"
#include "../visitors/visitor.hpp"
#include "./token.hpp"



class Statement
{
public:
    virtual llvm::Value *accept(BaseVisitor &visitor)
    {
        return nullptr;
    }
};

class ExpressionStatement : public Statement
{
public:
    UNIQUE_EXPRESSION expression;
    ExpressionStatement(UNIQUE_EXPRESSION &expression)
        : expression(std::move(expression)) {}

    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value *)
};

class PrintStatement : public Statement
{
public:
    UNIQUE_EXPRESSION expression;
    PrintStatement(UNIQUE_EXPRESSION &expression)
        : expression(std::move(expression)) {}

    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value *)
};

class LetStatement : public Statement
{
public:
    UNIQUE_EXPRESSION expression;
    std::string name;
    TokenTypes type;

    LetStatement(UNIQUE_EXPRESSION &expression, std::string name, TokenTypes type)
        : expression(std::move(expression)), name(name), type(type) {}

    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value *)
};

class BlockStatement : public Statement
{
public:
    std::vector<UNIQUE_STATEMENT> statements;
    BlockStatement(std::vector<UNIQUE_STATEMENT> &statements)
        : statements(std::move(statements)) {}

    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value *)
};

// doesnt inherit from statement because it doesnt have an accept method
class ElifStatement
{
public:
    UNIQUE_EXPRESSION condition;
    UNIQUE_STATEMENT branch;
    llvm::BasicBlock *block = nullptr;
    llvm::BasicBlock *codeBlock = nullptr;


    ElifStatement(UNIQUE_EXPRESSION &condition, UNIQUE_STATEMENT &branch)
        : condition(std::move(condition)), branch(std::move(branch)) {
        }
};

class IfStatement : public Statement
{
public:
    UNIQUE_EXPRESSION condition;
    UNIQUE_STATEMENT thenBranch;
    std::unique_ptr<std::vector<std::unique_ptr<ElifStatement>>> elifBranches;
    UNIQUE_STATEMENT elseBranch;

    IfStatement(UNIQUE_EXPRESSION &condition,
                UNIQUE_STATEMENT &thenBranch,
                std::unique_ptr<std::vector<std::unique_ptr<ElifStatement>>> &elifBranches,
                UNIQUE_STATEMENT &elseBranch)
        : condition(std::move(condition)),
          thenBranch(std::move(thenBranch)),
          elifBranches(std::move(elifBranches)),
          elseBranch(std::move(elseBranch)) {}

    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value *)
};