#pragma once
#include <vector>
#include <memory>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"

#include "llvm/IR/Function.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"

#include "visitor.hpp"
#include "env.hpp"

class Compiler : public BaseVisitor
{

    std::unique_ptr<llvm::LLVMContext> context;
    std::unique_ptr<llvm::Module> module;
    std::unique_ptr<llvm::IRBuilder<>> builder;

    Environment* env = new Environment();
    llvm::Function *mainFunction;

    llvm::Value *visit(IntExpression &expression) override;
    llvm::Value *visit(BinaryExpression &expression) override;
    llvm::Value *visit(VarExpression &statement) override;
    llvm::Value *visit(ExpressionStatement &atement) override;
    llvm::Value *visit(PrintStatement &statement) override;
    llvm::Value *visit(LetStatement &statement) override;
    llvm::Value *visit(BlockStatement &statement) override;
    llvm::Value *visit(IfStatement &statement) override;

    void setup();
    void createObjectFile();

public:
    void compile(std::vector<std::unique_ptr<Statement>> &statements);
    Compiler() : context(std::make_unique<llvm::LLVMContext>()),
                 module(std::make_unique<llvm::Module>("kscope", *context)),
                 builder(std::make_unique<llvm::IRBuilder<>>(*context)) {}
};
