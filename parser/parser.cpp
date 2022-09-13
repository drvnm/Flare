#include "parser.hpp"

bool Parser::atEnd()
{
    return tokens[current].type == TokenTypes::_EOF;
}

Token Parser::advance()
{
    if (!atEnd())
        current++;
    return peek();
}

Token Parser::previousToken()
{
    return tokens[current - 1];
}

Token Parser::peek()
{
    if (atEnd())
        return Token(_EOF, "", -1);
    return tokens[current];
}

Token Parser::peekNext()
{
    return tokens[current + 1];
}

Token Parser::consume(TokenTypes type, std::string message)
{

    if (peek().type == type)
    {
        advance();
        return previousToken();
    }

    error(message);
    return Token(_EOF, "", -1);
}

bool Parser::match(std::vector<TokenTypes> types)
{

    for (auto type : types)
    {

        if (peek().type == type)
        {
            advance();
            return true;
        }
    }
    return false;
}

void Parser::error(std::string message)
{
    std::cout << "Error: " << message << std::endl;
    exit(1);
}

Expression Parser::primary()
{
    if (match({NUMBER}))
    {

        int value = std::stoi(previousToken().lexeme);
        return LiteralExpression<int>(value);
    }
    else if (match({IDENTIFIER}))
    {
        return LiteralExpression<std::string>(peek().lexeme);
    }

    else
    {
        error("Expected expression");
        throw "Expected expression";
    }
}

Expression Parser::factor()
{
    Expression expr = primary();

    while (match({STAR, SLASH}))
    {
        Token op = peek();
        Expression right = primary();
        expr = BinaryExpression(expr, op, right);
    }

    return expr;
}

Expression Parser::term()
{
    Expression expr = factor();

    while (match({PLUS, MINUS}))
    {
        Token op = peek();
        Expression right = factor();
        expr = BinaryExpression(expr, op, right);
    }

    return expr;
}

Statement Parser::expressionStatement()
{
    Expression expr = term();
    consume(SEMICOLON, "Expected ';' after expression");
    return ExpressionStatement(expr);
}

Statement Parser::statement()
{

    return expressionStatement();
}

void Parser::program()
{
    while (!atEnd())
    {
        statements.push_back(
            std::make_unique<Statement>(statement()));
    }
}