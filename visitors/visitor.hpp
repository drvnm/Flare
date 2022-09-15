
#pragma once

#include "llvm/IR/Value.h"

class IntExpression;
class BinaryExpression;
class Statement;
class ExpressionStatement;
class PrintStatement;

// forward declare all the expressions and statements

class BaseVisitor
{
public:
    virtual llvm::Value *visit(IntExpression &expression) = 0;
    virtual llvm::Value *visit(BinaryExpression &expression) = 0;
    virtual llvm::Value *visit(ExpressionStatement &statement) = 0;
    virtual llvm::Value *visit(PrintStatement &statement) = 0;
};
