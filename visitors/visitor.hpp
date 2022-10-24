
#pragma once

#include "llvm/IR/Value.h"

class IntExpression;
class VarExpression;
class BinaryExpression;
class AssignmentExpression;
class CallExpression;
class Statement;
class ExpressionStatement;
class PrintStatement;
class LetStatement;
class BlockStatement;
class IfStatement;
class FnStatement;
class ReturnStatement;
class WhileStatement;

class BaseVisitor
{
public:
    virtual llvm::Value *visit(IntExpression &expression) = 0;
    virtual llvm::Value *visit(VarExpression &expression) = 0;
    virtual llvm::Value *visit(BinaryExpression &expression) = 0;
    virtual llvm::Value *visit(AssignmentExpression &expression) = 0;
    virtual llvm::Value *visit(CallExpression &expression) = 0;
    virtual llvm::Value *visit(ExpressionStatement &statement) = 0;
    virtual llvm::Value *visit(PrintStatement &statement) = 0;
    virtual llvm::Value *visit(LetStatement &statement) = 0;
    virtual llvm::Value *visit(BlockStatement &statement) = 0;
    virtual llvm::Value *visit(IfStatement &statement) = 0;
    virtual llvm::Value *visit(FnStatement &statement) = 0;
    virtual llvm::Value *visit(ReturnStatement &statement) = 0;
    virtual llvm::Value *visit(WhileStatement &statement) = 0;
};
