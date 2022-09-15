#pragma once
#include <vector>
#include <memory>
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
// include llvm::outs
#include "llvm/Support/raw_ostream.h"

#include "visitor.hpp"

class Compiler : public BaseVisitor
{

    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;

    llvm::Value *visit(IntExpression &expression) override;
    llvm::Value *visit(BinaryExpression &expression) override;
    llvm::Value *visit(ExpressionStatement &atement) override;
    llvm::Value*  visit(PrintStatement &statement) override;

public:
    int compile(std::vector<std::unique_ptr<Statement>> &statements);
    Compiler() : builder(context) {}
};
