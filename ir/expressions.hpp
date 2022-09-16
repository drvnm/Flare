#pragma once

#include <memory>

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
    std::unique_ptr<Expression> left;
    std::unique_ptr<Expression> right;
    Token op;
    BinaryExpression(
        std::unique_ptr<Expression> &left,
        Token op,
        std::unique_ptr<Expression> &right)
        : left(std::move(left)),
          right(std::move(right)),
          op(op) {}

     ACCEPT_VISITOR_METHOD_HEADER(llvm::Value*)
};
