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

Token Parser::type()
{
    if (match({I8, I16, I32, I64, U8, U16, U32, U64}))
    {
        return previousToken();
    }
  
    error("Expected a type");
    return Token(_EOF, "", -1);
}

std::unique_ptr<Expression> Parser::primary()
{
    if (match({NUMBER}))
    {

        int value = std::stoi(previousToken().lexeme);
        return std::make_unique<IntExpression>(value);
    }
    else if (match({IDENTIFIER}))
    {
        return std::make_unique<VarExpression>(previousToken().lexeme);
    }

    else
    {
        error("Expected expression");
        return nullptr;
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

std::unique_ptr<Statement> Parser::printStatement()
{
    auto expr = term();
    consume(SEMICOLON, "Expected ';' after print statement");
    return std::make_unique<PrintStatement>(expr);
}

std::unique_ptr<Statement> Parser::letStatement()
{
    consume(IDENTIFIER, "Expected identifier after 'let' keyword");
    std::string name = previousToken().lexeme;
    consume(COLON, "Expected ';' after identifier in 'let' statement");
    Token size = type();
    consume(EQUAL, "Expected '=' after type in 'let' statement");
    auto expr = term();
    consume(SEMICOLON, "Expected ';' after expression");
    return std::make_unique<LetStatement>(expr, name, size);
}

std::unique_ptr<Statement> Parser::statement()
{
    if (match({PRINT}))
    {
        return printStatement();
    }
    else if (match({LET}))
    {
        return letStatement();
    }
   
    else
    {
        return expressionStatement();
    }
}

void Parser::program()
{
    while (!atEnd())
    {
        statements.push_back(statement());
    }
}