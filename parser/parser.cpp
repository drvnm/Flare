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

TokenTypes Parser::type()
{
    if (match({I8, I16, I32, I64, U8, U16, U32, U64}))
    {
        return previousToken().type;
    }

    error("Expected a type");
    return Token(_EOF, "", -1).type;
}

UNIQUE_EXPRESSION Parser::primary()
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

UNIQUE_EXPRESSION Parser::factor()
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

UNIQUE_EXPRESSION Parser::term()
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

UNIQUE_EXPRESSION Parser::comparison()
{
    auto expr = term();

    while (match({LESS, LESS_EQUAL, GREATER, GREATER_EQUAL}))
    {
        Token op = previousToken();
        auto right = term();
        expr = std::make_unique<BinaryExpression>(expr, op, right);
    }

    return expr;
}

UNIQUE_EXPRESSION Parser::equality()
{
    auto expr = comparison();

    while (match({EQUAL_EQUAL, NOT_EQUAL}))
    {
        Token op = previousToken();
        auto right = comparison();
        expr = std::make_unique<BinaryExpression>(expr, op, right);
    }

    return expr;
}

UNIQUE_STATEMENT Parser::ifStatement()
{
    auto condition = equality();
    auto thenBranch = statement();
    UNIQUE_STATEMENT elseBranch = nullptr;
    std::unique_ptr<std::vector<std::unique_ptr<ElifStatement>>> elifBranches = std::make_unique<std::vector<std::unique_ptr<ElifStatement>>>();

    while (match({ELIF}))
    {
        auto elifCondition = equality();
        auto elifBranch = statement();
        auto elifStatement = std::make_unique<ElifStatement>(elifCondition, elifBranch);
        elifBranches->push_back(std::move(elifStatement));
    }

    if (match({ELSE}))
    {
        elseBranch = statement();
    }

    return std::make_unique<IfStatement>(condition,
                                         thenBranch,
                                         elifBranches,
                                         elseBranch);
}

UNIQUE_STATEMENT Parser::blockStatement()
{
    std::vector<UNIQUE_STATEMENT> statements;

    while (!atEnd() && peek().type != RBRACE)
    {
        statements.push_back(statement());
    }

    consume(RBRACE, "Expected '}' after block");

    return std::make_unique<BlockStatement>(statements);
}

UNIQUE_STATEMENT Parser::expressionStatement()
{
    auto expr = equality();
    consume(SEMICOLON, "Expected ';' after expression");
    return std::make_unique<ExpressionStatement>(expr);
}

UNIQUE_STATEMENT Parser::printStatement()
{
    auto expr = equality();
    consume(SEMICOLON, "Expected ';' after print statement");
    return std::make_unique<PrintStatement>(expr);
}

UNIQUE_STATEMENT Parser::letStatement()
{
    consume(IDENTIFIER, "Expected identifier after 'let' keyword");
    std::string name = previousToken().lexeme;
    consume(COLON, "Expected ';' after identifier in 'let' statement");
    TokenTypes size = type();
    consume(EQUAL, "Expected '=' after type in 'let' statement");
    auto expr = equality();
    consume(SEMICOLON, "Expected ';' after expression");
    return std::make_unique<LetStatement>(expr, name, size);
}

UNIQUE_STATEMENT Parser::statement()
{
    if (match({PRINT}))
    {
        return printStatement();
    }
    else if (match({LBRACE}))
    {
        return blockStatement();
    }

    else if (match({LET}))
    {
        return letStatement();
    }
    else if (match({IF}))
    {
        return ifStatement();
    }

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