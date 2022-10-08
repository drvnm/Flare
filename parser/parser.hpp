#pragma once
#include <string>
#include <vector>
#include <memory>

#include "../ir/token.hpp"
#include "../ir/expressions.hpp"
#include "../ir/statements.hpp"

#define UNIQUE_EXPRESSION std::unique_ptr<Expression>
#define UNIQUE_STATEMENT std::unique_ptr<Statement>

class Parser
{
    std::vector<Token> tokens;
    int current = 0;

    bool atEnd();
    Token currentToken();
    Token advance();
    Token previousToken();
    Token peek();
    Token peekNext();
    Token consume(TokenTypes type, std::string message);
    bool match(std::vector<TokenTypes> types);
    void error(std::string message);

    TokenTypes type();

    UNIQUE_EXPRESSION primary();
    UNIQUE_EXPRESSION factor();
    UNIQUE_EXPRESSION term();
    UNIQUE_EXPRESSION comparison();
    UNIQUE_EXPRESSION equality();
    UNIQUE_EXPRESSION assignment();
    UNIQUE_EXPRESSION expression();

    UNIQUE_STATEMENT letStatement();
    UNIQUE_STATEMENT blockStatement();
    UNIQUE_STATEMENT expressionStatement();
    UNIQUE_STATEMENT printStatement();
    UNIQUE_STATEMENT ifStatement();
    UNIQUE_STATEMENT whileStatement();
    UNIQUE_STATEMENT statement();

public:
    void program();
    Parser(std::vector<Token> tokens) : tokens(tokens) {}
    std::vector<UNIQUE_STATEMENT> statements;
};