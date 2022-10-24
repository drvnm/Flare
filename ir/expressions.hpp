#pragma once

#include <memory>
#include <vector>
#include "token.hpp"
#include "llvm/IR/Value.h"

#include "../visitors/visitor.hpp"
#include "./common.hpp"

class Expression
{
public:
    virtual llvm::Value* accept(BaseVisitor &visitor)
    {
        return nullptr;
    }
};

class IntExpression : public Expression
{
public:
    int value;
    IntExpression(int value) : value(value) {}
    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value*)
};

class VarExpression : public Expression
{
public:
    std::string name;
    VarExpression(std::string name) : name(name) {}
    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value*)
};

class BinaryExpression : public Expression
{
public:
    UNIQUE_EXPRESSION left;
    UNIQUE_EXPRESSION right;
    Token op;
    BinaryExpression(
        UNIQUE_EXPRESSION &left,
        Token op,
        UNIQUE_EXPRESSION &right)
        : left(std::move(left)),
          right(std::move(right)),
          op(op) {}

     ACCEPT_VISITOR_METHOD_HEADER(llvm::Value*)
};

class AssignmentExpression : public Expression
{
    public:
    UNIQUE_EXPRESSION value;
    std::string name;
    AssignmentExpression(std::string name, UNIQUE_EXPRESSION &value)
        : value(std::move(value)), name(name) {}
    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value*)
};

class CallExpression : public Expression
{
public:
    std::string name;
    std::vector<UNIQUE_EXPRESSION> args;
    CallExpression(std::string name, std::vector<UNIQUE_EXPRESSION>& args)
        : name(name), args(std::move(args)) {}
    ACCEPT_VISITOR_METHOD_HEADER(llvm::Value*)
};