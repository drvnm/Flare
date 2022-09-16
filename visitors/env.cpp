#include "env.hpp"

void Environment::define(std::string name, llvm::Value *value)
{
    values[name] = value;
}

llvm::Value *Environment::get(std::string name)
{
    if (values.find(name) != values.end())
    {
        return values[name];
    }

    return nullptr;
}

void Environment::assign(std::string name, llvm::Value *value)
{
    if (values.find(name) != values.end())
    {
        values[name] = value;
        return;
    }
}