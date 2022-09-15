#include "compiler.hpp"

#include "../ir/token.hpp"
#include "../ir/expressions.hpp"
#include "../ir/statements.hpp"

llvm::Value *Compiler::visit(IntExpression &expression)
{

    return llvm::ConstantInt::get(context, llvm::APInt(64, expression.value, true));
}

llvm::Value *Compiler::visit(BinaryExpression &expression)
{
    llvm::Value *left = expression.left->accept(*this);
    llvm::Value *right = expression.right->accept(*this);

    switch (expression.op.type)
    {
    case PLUS:
        return builder.CreateAdd(left, right, "addtmp");
    case MINUS:
        return builder.CreateSub(left, right, "subtmp");
    case STAR:
        return builder.CreateMul(left, right, "multmp");
    case SLASH:
        return builder.CreateSDiv(left, right, "divtmp");
    default:
        return nullptr;
    }
}

llvm::Value *Compiler::visit(ExpressionStatement &statement)
{

    return statement.expression->accept(*this);
}

llvm::Value* Compiler::visit(PrintStatement &statement)
{

    llvm::Value *value = statement.expression->accept(*this);
    value->print(llvm::outs());
    return nullptr;
}

int Compiler::compile(std::vector<std::unique_ptr<Statement>> &statements)
{
    for (auto &statement : statements)
    {

        statement->accept(*this);
    }
    return 0;
}