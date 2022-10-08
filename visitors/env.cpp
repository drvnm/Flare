#include "env.hpp"

void Environment::error(std::string message, int line)
{
    std::cout << "Error: " << message << " at line " << line << std::endl;
    exit(1);
}

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
    if (enclosing != nullptr)
    {
        return enclosing->get(name);
    }
    error("Undefined variable '" + name + "'", 0);
    return nullptr;
}

void Environment::assign(std::string name, llvm::Value *value)
{
    if (values.find(name) != values.end())
    {
        values[name] = value;
        return;
    }
    if (enclosing != nullptr)
    {
        enclosing->assign(name, value);
        return;
    }
    error("Undefined variable '" + name + "'", 0);
}