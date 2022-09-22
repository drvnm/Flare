
#pragma once

#include "llvm/IR/Value.h"

class IntExpression;
class VarExpression;
class BinaryExpression;
class Statement;
class ExpressionStatement;
class PrintStatement;
class LetStatement;
class BlockStatement;
class IfStatement;

class BaseVisitor
{
public:
    virtual llvm::Value *visit(IntExpression &expression) = 0;
    virtual llvm::Value *visit(VarExpression &expression) = 0;
    virtual llvm::Value *visit(BinaryExpression &expression) = 0;
    virtual llvm::Value *visit(ExpressionStatement &statement) = 0;
    virtual llvm::Value *visit(PrintStatement &statement) = 0;
    virtual llvm::Value *visit(LetStatement &statement) = 0;
    virtual llvm::Value *visit(BlockStatement &statement) = 0;
    virtual llvm::Value *visit(IfStatement &statement) = 0;
};
