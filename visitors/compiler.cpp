#include "compiler.hpp"

#include "../ir/token.hpp"
#include "../ir/expressions.hpp"
#include "../ir/statements.hpp"

std::map<TokenTypes, std::pair<int, bool>> typeMap = {
    {I8, {8, true}},
    {I16, {16, true}},
    {I32, {32, true}},
    {I64, {64, true}},
    {U8, {8, false}},
    {U16, {16, false}},
    {U32, {32, false}},
    {U64, {64, false}},
};

llvm::Value *Compiler::visit(IntExpression &expression)
{

    return llvm::ConstantInt::get(*context, llvm::APInt(64, expression.value, true));
}

llvm::Value *Compiler::visit(VarExpression &statement)
{
    return env->get(statement.name);
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
    case EQUAL_EQUAL:
        return builder->CreateICmpEQ(left, right, "eqtmp");
    case NOT_EQUAL:
        return builder->CreateICmpNE(left, right, "netmp");
    case LESS:
        return builder->CreateICmpSLT(left, right, "lttmp");
    case LESS_EQUAL:
        return builder->CreateICmpSLE(left, right, "letmp");
    case GREATER:
        return builder->CreateICmpSGT(left, right, "gttmp");
    case GREATER_EQUAL:
        return builder->CreateICmpSGE(left, right, "getmp");
    default:
        return nullptr;
    }
}

llvm::Value *Compiler::visit(ExpressionStatement &statement)
{

    auto value = statement.expression->accept(*this);
    return value;
}

llvm::Value *Compiler::visit(PrintStatement &statement)
{

    llvm::Value *value = statement.expression->accept(*this);
    value->print(llvm::outs());
    std::cout << std::endl;
    return nullptr;
}

llvm::Value *Compiler::visit(LetStatement &statement)
{
    int bitWidth;
    bool isSigned;

    std::tie(bitWidth, isSigned) = typeMap[statement.type];


    llvm::Value *value = statement.expression->accept(*this);
    llvm::Value *newValue = builder->CreateIntCast(value, llvm::Type::getIntNTy(*context, bitWidth), isSigned);
    env->define(statement.name, newValue);

    return newValue;
}


llvm::Value* Compiler::visit(BlockStatement &statement) {
    llvm::BasicBlock *innerBlock = llvm::BasicBlock::Create(*context, "innerBlock", mainFunction);
    builder->SetInsertPoint(innerBlock);

    // TODO: stop using new for envs
    Environment* newEnv = new Environment(env);
    env = newEnv;

    for (auto &stmt : statement.statements) {
        stmt->accept(*this);
    }

    env = env->enclosing;

    return nullptr;
}

int Compiler::compile(std::vector<std::unique_ptr<Statement>> &statements)
{
    mainFunction = llvm::Function::Create(
        llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false),
        llvm::Function::ExternalLinkage,
        "main",
        module.get());
    llvm::BasicBlock *block = llvm::BasicBlock::Create(*context, "entry", mainFunction);
    builder->SetInsertPoint(block);



    for (auto &statement : statements)
    {
        auto x = statement->accept(*this);
    }
    module->print(llvm::outs(), nullptr);
    return 0;
}
