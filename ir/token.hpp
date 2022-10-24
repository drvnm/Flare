#pragma once

#include <string>
#include <iostream>

enum TokenTypes
{
    // COMPILER TYOES
    NUMBER,

    I8,
    I16,
    I32,
    I64,
    U8,
    U16,
    U32,
    U64,

    PLUS,
    MINUS,
    SLASH,
    STAR,
    EQUAL,

    EQUAL_EQUAL,
    NOT_EQUAL,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,

    LBRACKET,
    RBRACKET,
    LBRACE,
    RBRACE,

    // KEYWORDS
    PRINT,
    LET,
    IF,
    ELIF,
    ELSE,
    WHILE,
    FN,
    RETURN,

    IDENTIFIER,

    SEMICOLON,
    COMMA,
    _EOF,
    COLON
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
