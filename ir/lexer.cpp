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
    while (!atEnd() && isalpha(currentChar()) || currentChar() == '_')

    {
        word += std::string(1, currentChar());
        advance();
    }
    if (word == "print")
    {
        tokens.push_back(Token(PRINT, word, line));
    }
    else if (word == "let")
    {
        tokens.push_back(Token(LET, word, line));
    }

    else
    {
        tokens.push_back(Token(IDENTIFIER, word, line));
    }
}

void Lexer::tryType()
{
    std::string type;
    while (!atEnd() && isalnum(currentChar()))
    {
        type += std::string(1, currentChar());
        advance();
    }
    if (type == "i8")
    {
        tokens.push_back(Token(I8, type, line));
    }
    else if (type == "i16")
    {
        tokens.push_back(Token(I16, type, line));
    }
    else if (type == "i32")
    {
        tokens.push_back(Token(I32, type, line));
    }
    else if (type == "i64")
    {
        tokens.push_back(Token(I64, type, line));
    }
    else if (type == "u8")
    {
        tokens.push_back(Token(U8, type, line));
    }
    else if (type == "u16")
    {
        tokens.push_back(Token(U16, type, line));
    }
    else if (type == "u32")
    {
        tokens.push_back(Token(U32, type, line));
    }
    else if (type == "u64")
    {
        tokens.push_back(Token(U64, type, line));
    }
    else
    {
        tokens.push_back(Token(IDENTIFIER, type, line));
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

        else if (curr == 'i' || curr == 'u')
        {
            tryType();
        }
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

        else if (curr == ';')
        {
            tokens.push_back(Token(SEMICOLON, std::string(1, curr), line));
            advance();
        }

        else if (curr == ':')
        {
            tokens.push_back(Token(COLON, std::string(1, curr), line));
            advance();
        }

        else if (curr == '=')
        {
            tokens.push_back(Token(EQUAL, std::string(1, curr), line));
            advance();
        }

        else
        {
            std::cout << "Unexpected character: " << curr << std::endl;
            advance();
        }

       
    }

    tokens.push_back(Token(_EOF, "EOF", line));
}