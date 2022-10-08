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

// CASTS [val1] TO [val2] 
llvm::Value *Compiler::CastValueToVarType(llvm::Value *val1, llvm::Value* val2)
{
    auto type1 = val1->getType();
    auto type2 = val2->getType();

    if (type1 == type2)
        return val1;

    if (type1->isIntegerTy() && type2->isIntegerTy())
    {
        auto type1Size = type1->getIntegerBitWidth();
        auto type2Size = type2->getIntegerBitWidth();

        if (type1Size > type2Size)
            return builder->CreateIntCast(val1, type2, true);
        else
            return builder->CreateIntCast(val1, type2, false);
    }

    return val1;
}

llvm::Value *Compiler::visit(IntExpression &expression)
{
    return llvm::ConstantInt::get(*context, llvm::APInt(64, expression.value, true));
}

llvm::Value *Compiler::visit(VarExpression &statement)
{
    auto var = env->get(statement.name);
    return builder->CreateLoad(var);
}

llvm::Value *Compiler::visit(BinaryExpression &expression)
{
    llvm::Value *left = expression.left->accept(*this);
    llvm::Value *right = expression.right->accept(*this);

    left = CastValueToVarType(left, right);
    right = CastValueToVarType(right, left);
    
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

llvm::Value *Compiler::visit(AssignmentExpression &statement)
{
    auto *value = statement.value->accept(*this);
    auto var = env->get(statement.name);
    value = CastValueToVarType(value, var);
    return builder->CreateStore(value, var);
}

llvm::Value *Compiler::visit(ExpressionStatement &statement)
{

    auto value = statement.expression->accept(*this);
    return value;
}

llvm::Value *Compiler::visit(PrintStatement &statement)
{

    llvm::Function *printFunc = module->getFunction("printf");
    std::vector<llvm::Value *> args;
    auto value = statement.expression->accept(*this);
    args.push_back(builder->CreateGlobalStringPtr("%d\n"));
    args.push_back(value);
    builder->CreateCall(printFunc, args, "printf");

    return nullptr;
}

llvm::Value *Compiler::visit(LetStatement &statement)
{
    int bitWidth;
    bool isSigned;

    std::tie(bitWidth, isSigned) = typeMap[statement.type];

    llvm::Value *value = statement.expression->accept(*this);
    llvm::Value *newValue = builder->CreateIntCast(value, llvm::Type::getIntNTy(*context, bitWidth), isSigned);
    llvm::AllocaInst *alloca = builder->CreateAlloca(llvm::Type::getIntNTy(*context, bitWidth), 0, statement.name);
    builder->CreateStore(newValue, alloca);
    env->define(statement.name, alloca);
    return newValue;
}

llvm::Value *Compiler::visit(BlockStatement &statement)
{
    // TODO: stop using new for envs
    Environment *newEnv = new Environment(env);
    env = newEnv;
    for (auto &stmt : statement.statements)
    {
        stmt->accept(*this);
    }
    env = env->enclosing;
    return nullptr;
}

llvm::Value *Compiler::visit(IfStatement &statement)
{
    int currentIf = 0;

    llvm::Value *condition = statement.condition->accept(*this);
    llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(*context, "then", mainFunction);
    llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create(*context, "else");
    llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create(*context, "ifcont");
    llvm::BasicBlock *nextBlock;

    for (int i = 0; i < statement.elifBranches->size(); i++)
    {
        ElifStatement &elif = *(statement.elifBranches->at(i));
        elif.block = llvm::BasicBlock::Create(*context, "elif");
        elif.codeBlock = llvm::BasicBlock::Create(*context, "elifcont");
        currentIf++;
    }

    if (statement.elifBranches->size())
        nextBlock = statement.elifBranches->at(0).get()->block;
    else if (statement.elseBranch)
        nextBlock = elseBlock;
    else
        nextBlock = mergeBlock;

    builder->CreateCondBr(condition, thenBlock, nextBlock);

    builder->SetInsertPoint(thenBlock);
    statement.thenBranch->accept(*this);
    builder->CreateBr(mergeBlock);

    for (int i = 0; i < statement.elifBranches->size(); i++)
    {

        ElifStatement &elif = *(statement.elifBranches->at(i));
        llvm::Value *elifCondition = elif.condition->accept(*this);
        nextBlock = i == statement.elifBranches->size() - 1 ? (statement.elseBranch ? elseBlock : mergeBlock) : statement.elifBranches->at(i + 1).get()->block;

        elif.block->insertInto(mainFunction);
        builder->SetInsertPoint(elif.block);
        builder->CreateCondBr(elifCondition, elif.codeBlock, nextBlock);
        elif.codeBlock->insertInto(mainFunction);
        builder->SetInsertPoint(elif.codeBlock);
        elif.branch->accept(*this);

        builder->CreateBr(mergeBlock);
    }

    if (statement.elseBranch)
    {
        elseBlock->insertInto(mainFunction);
        builder->SetInsertPoint(elseBlock);
        statement.elseBranch->accept(*this);
        builder->CreateBr(mergeBlock);
    }

    mergeBlock->insertInto(mainFunction);
    builder->SetInsertPoint(mergeBlock);

    return nullptr;
}

llvm::Value *Compiler::visit(WhileStatement &statement)
{
    llvm::BasicBlock *loopBlock = llvm::BasicBlock::Create(*context, "loop", mainFunction);
    llvm::BasicBlock *loopBody = llvm::BasicBlock::Create(*context, "loopBody");
    llvm::BasicBlock *afterBlock = llvm::BasicBlock::Create(*context, "afterloop");

    builder->CreateBr(loopBlock);
    builder->SetInsertPoint(loopBlock);

    llvm::Value *condition = statement.condition->accept(*this);
    builder->CreateCondBr(condition, loopBody, afterBlock);

    loopBody->insertInto(mainFunction);
    builder->SetInsertPoint(loopBody);
    statement.branch->accept(*this);
    builder->CreateBr(loopBlock);

    afterBlock->insertInto(mainFunction);
    builder->SetInsertPoint(afterBlock);

    return nullptr;
}

void Compiler::compile(std::vector<std::unique_ptr<Statement>> &statements)
{

    setup(); // sets up predefined functions
    for (auto &statement : statements)
    {
        auto x = statement->accept(*this);
    }

    // return statement for the main function
    builder->CreateRetVoid();
    module->print(llvm::outs(), nullptr);
    createObjectFile();
}

void Compiler::setup()
{
    std::vector<llvm::Type *> printfArgs;
    printfArgs.push_back(llvm::Type::getInt8PtrTy(*context));
    llvm::FunctionType *printfType = llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), printfArgs, true);
    llvm::Function::Create(printfType, llvm::Function::ExternalLinkage, llvm::Twine("printf"), module.get());

    mainFunction = llvm::Function::Create(
        llvm::FunctionType::get(llvm::Type::getVoidTy(*context), false),
        llvm::Function::ExternalLinkage,
        "main",
        module.get());
    llvm::BasicBlock *block = llvm::BasicBlock::Create(*context, "entry", mainFunction);
    builder->SetInsertPoint(block);
}

void Compiler::createObjectFile()
{

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    module->setTargetTriple(targetTriple);

    std::string Error;
    auto Target = llvm::TargetRegistry::lookupTarget(targetTriple, Error);

    if (!Target)
    {
        llvm::errs() << Error;
        return;
    }

    auto CPU = "generic";
    auto Features = "";

    llvm::TargetOptions opt;
    auto RM = llvm::Optional<llvm::Reloc::Model>();
    auto targetMachine = Target->createTargetMachine(targetTriple, CPU, Features, opt, RM);

    module->setDataLayout(targetMachine->createDataLayout());

    auto fileName = "output.o";
    std::error_code EC;
    llvm::raw_fd_ostream dest(fileName, EC, llvm::sys::fs::OF_None);

    if (EC)
    {
        llvm::errs() << "Could not open file: " << EC.message();
        return;
    }

    llvm::legacy::PassManager pass;
    auto FileType = llvm::CGFT_ObjectFile;

    if (targetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType))
    {
        llvm::errs() << "TheTargetMachine can't emit a file of this type";
        return;
    }

    pass.run(*module);
    dest.flush();
}
