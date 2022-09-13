#pragma once
#include <string>
#include <iostream>

enum TokenTypes
{
    // COMPILER TYOES
    NUMBER,

    PLUS,
    MINUS,
    SLASH,
    STAR,

    // KEYWORDS
    PRINT,

    IDENTIFIER,

    SEMICOLON, _EOF,
};

class Token
{
public:
    TokenTypes type;
    std::string lexeme;
    int line;

    Token(TokenTypes type, std::string lexeme, int line) : type(type), lexeme(lexeme), line(line)
    {
    }
    void print();
};
