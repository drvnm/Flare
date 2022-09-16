#pragma once
#include <map>
#include "llvm/IR/Value.h"

class Environment
{

    std::map<std::string, llvm::Value *> values;

public:
    Environment() = default;

    void define(std::string name, llvm::Value *value);
    llvm::Value *get(std::string name);
    void assign(std::string name, llvm::Value *value);
};