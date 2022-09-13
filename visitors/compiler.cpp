#include "compiler.hpp"

#include "../ir/token.hpp"
#include "../ir/expressions.hpp"
#include "../ir/statements.hpp"

int Compiler::visit(LiteralExpression<int> expression)
{
    return expression.value;
}

int Compiler::visit(BinaryExpression expression)
{
    int left = expression.left.accept(*this);

    int right = expression.right.accept(*this);
    switch (expression.op.type)
    {
    case PLUS:
        return left + right;
    case MINUS:
        return left - right;
    case STAR:
        return left * right;
    case SLASH:
        return left / right;
    default:
        return 0;
    }
}

int Compiler::visit(ExpressionStatement statement)
{

    return statement.expression.accept(*this);
}

int Compiler::compile(std::vector<std::unique_ptr<Statement>>& statements) 
{    
  for (auto& statement : statements) 
  {

    int result = statement->accept(*this);
    std::cout << result << std::endl;
  }
  return 0;
}