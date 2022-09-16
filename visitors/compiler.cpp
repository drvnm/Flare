#include "compiler.hpp"

#include "../ir/token.hpp"
#include "../ir/expressions.hpp"
#include "../ir/statements.hpp"

llvm::Value *Compiler::visit(IntExpression &expression)
{

    return llvm::ConstantInt::get(*context, llvm::APInt(64, expression.value, true));
}

llvm::Value *Compiler::visit(VarExpression &statement)
{
    return env.get(statement.name);
}

llvm::Value *Compiler::visit(BinaryExpression &expression)
{
    llvm::Value *left = expression.left->accept(*this);
    llvm::Value *right = expression.right->accept(*this);

    switch (expression.op.type)
    {
    case PLUS:
        return builder->CreateAdd(left, right, "addtmp");
    case MINUS:
        return builder->CreateSub(left, right, "subtmp");
    case STAR:
        return builder->CreateMul(left, right, "multmp");
    case SLASH:
        return builder->CreateSDiv(left, right, "divtmp");
    default:
        return nullptr;
    }
}

llvm::Value *Compiler::visit(ExpressionStatement &statement)
{

    auto value = statement.expression->accept(*this);
    // print IR from value 
    value->print(llvm::outs());
    return value;
}

llvm::Value *Compiler::visit(PrintStatement &statement)
{

    llvm::Value *value = statement.expression->accept(*this);
    value->print(llvm::outs());
    return nullptr;
}

llvm::Value *Compiler::visit(LetStatement &statement)
{
    int bitWidth;
    bool isSigned;
    // yeah yeah ill change this later
    if (statement.type.type == U8)
    {
        bitWidth = 8;
        isSigned = false;
    }
    else if (statement.type.type == U16)
    {
        bitWidth = 16;
        isSigned = false;
    }
    else if (statement.type.type == U32)
    {
        bitWidth = 32;
        isSigned = false;
    }
    else if (statement.type.type == U64)
    {
        bitWidth = 64;
        isSigned = false;
    }
    else if (statement.type.type == I8)
    {
        bitWidth = 8;
        isSigned = true;
    }
    else if (statement.type.type == I16)
    {
        bitWidth = 16;
        isSigned = true;
    }
    else if (statement.type.type == I32)
    {
        bitWidth = 32;
        isSigned = true;
    }
    else if (statement.type.type == I64)
    {
        bitWidth = 64;
        isSigned = true;
    }

    llvm::Value *value = statement.expression->accept(*this);
    llvm::Value *newValue = builder->CreateIntCast(value, llvm::Type::getIntNTy(*context, bitWidth), isSigned);
    env.define(statement.name, newValue);
    // print IR from value 
    newValue->print(llvm::outs());
    return newValue;
}

int Compiler::compile(std::vector<std::unique_ptr<Statement>> &statements)
{
    for (auto &statement : statements)
    {
        auto x = statement->accept(*this);
     
    }
    module->print(llvm::outs(), nullptr);
    return 0;
}

