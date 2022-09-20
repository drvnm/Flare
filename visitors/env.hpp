#pragma once
#include <map>
#include <string>
#include <iostream>
#include "llvm/IR/Value.h"

class Environment
{
    std::map<std::string, llvm::Value *> values;
    void error(std::string message, int line);

public:
    Environment *enclosing;
    Environment() = default;
    Environment(Environment *enclosing) : enclosing(enclosing) {}
    void define(std::string name, llvm::Value *value);
    llvm::Value *get(std::string name);
    void assign(std::string name, llvm::Value *value);
};