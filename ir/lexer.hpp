#pragma once
#include <string>
#include <vector>

#include "token.hpp"

class Lexer
{
    int current = 0;
    int line = 0;
    std::string& source;
    bool atEnd();
    char currentChar();
    void advance();
    void skipWhiteSpace();

    void number();
    void word();
    void tryType();

public:
    std::vector<Token> tokens;
    Lexer(std::string &source) : source(source) {}
    void lex();
};