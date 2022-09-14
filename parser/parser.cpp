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

std::unique_ptr<Expression> Parser::primary()
{
    if (match({NUMBER}))
    {

        int value = std::stoi(previousToken().lexeme);
        return std::make_unique<LiteralExpression<int>>(value);
    }
    // else if (match({IDENTIFIER}))
    // {
    //     return LiteralExpression<std::string>(peek().lexeme);
    // }

    else
    {
        error("Expected expression");
        throw "Expected expression";
    }
}

std::unique_ptr<Expression> Parser::factor()
{
    auto expr = primary();

    while (match({STAR, SLASH}))
    {
        Token op = previousToken();
        auto right = primary();
        expr = std::make_unique<BinaryExpression>(expr, op, right);
    }

    return expr;
}

std::unique_ptr<Expression> Parser::term()
{
    auto expr = factor();

    while (match({PLUS, MINUS}))
    {
        Token op = previousToken();
        auto right = factor();
        expr = std::make_unique<BinaryExpression>(expr, op, right);
    }

    return expr;
}

std::unique_ptr<Statement> Parser::expressionStatement()
{
    auto expr = term();
    consume(SEMICOLON, "Expected ';' after expression");
    return std::make_unique<ExpressionStatement>(expr);
}

std::unique_ptr<Statement> Parser::statement()
{

    return expressionStatement();
}

void Parser::program()
{
    while (!atEnd())
    {
        statements.push_back(statement());
    }
}