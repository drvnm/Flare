#pragma once
#include <string>
#include <iostream>

enum TokenTypes
{
    // COMPILER TYOES
    NUMBER,

    I8, I16, I32, I64,
    U8, U16, U32, U64,

    PLUS,
    MINUS,
    SLASH,
    STAR,
    EQUAL,

    // KEYWORDS
    PRINT, LET,

    IDENTIFIER,

    SEMICOLON, _EOF, COLON
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
