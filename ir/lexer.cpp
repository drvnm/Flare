#include "lexer.hpp"
#include <iostream>
bool Lexer::atEnd()
{
    return current >= source.length();
}

char Lexer::currentChar()
{
    return source[current];
}
void Lexer::advance()
{
    current++;
}

void Lexer::skipWhiteSpace()
{
    while (!atEnd() && isspace(currentChar()))
    {
        advance();
    }
}

void Lexer::number()
{
    std::string number;
    while (!atEnd() && isdigit(currentChar()))
    {
        number += std::string(1, currentChar());
        advance();
    }
    tokens.push_back(Token(NUMBER, number, line));
}

void Lexer::word()
{
    std::string word;
    while (!atEnd() && isalpha(currentChar()))
    {
        word += std::string(1, currentChar());
        advance();
    }
    if (word == "print")
    {
        tokens.push_back(Token(PRINT, word, line));
    }
    else
    {
        tokens.push_back(Token(IDENTIFIER, word, line));
    }
}

void Lexer::lex()
{
    while (!atEnd())
    {
        char curr = currentChar();
        if (curr == ' ')
            skipWhiteSpace();

        else if (curr == '\n')
            line += 1;

        else if (isdigit(curr))
            number();

        // if alpha, identifier
        else if (isalpha(curr))
            word();

        // single char operators
        else if (curr == '+')
        {
            tokens.push_back(Token(PLUS, std::string(1, curr), line));
            advance();
        }
        else if (curr == '-')
        {
            tokens.push_back(Token(MINUS, std::string(1, curr), line));
            advance();
        }
        else if (curr == '*')
        {
            tokens.push_back(Token(STAR, std::string(1, curr), line));
            advance();
        }
        else if (curr == '/')
        {
            tokens.push_back(Token(SLASH, std::string(1, curr), line));
            advance();
        }

        // multi char operators
        else if (curr == ';')
        {
            tokens.push_back(Token(SEMICOLON, std::string(1, curr), line));
            advance();
        }

        else
        {
            std::cout << "Error: Unexpected character: " << currentChar() << std::endl;
            exit(1);
        }
    }

    tokens.push_back(Token(_EOF, "EOF", line));
}