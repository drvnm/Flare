#include "lexer.hpp"
#include <iostream>
#include <map>

std::map<std::string, TokenTypes> singleCharTokens = {
    {"+", PLUS},
    {"-", MINUS},
    {"/", SLASH},
    {"*", STAR},
    {"=", EQUAL},
    {";", SEMICOLON},
    {":", COLON},
    {"<", LESS},
    {">", GREATER},
    {"(", LBRACKET},
    {")", RBRACKET},
    {"{", LBRACE},
    {"}", RBRACE},

};

std::map<std::string, TokenTypes> mulicharTokens = {
    {"==", EQUAL_EQUAL},
    {"!=", NOT_EQUAL},
    {"<=", LESS_EQUAL},
    {">=", GREATER_EQUAL},
    {"<", LESS},
    {">", GREATER},
};

std::map<std::string, TokenTypes> keywords = {
    {"print", PRINT},
    {"let", LET},
    {"i8", I8},
    {"i16", I16},
    {"i32", I32},
    {"i64", I64},
    {"u8", U8},
    {"u16", U16},
    {"u32", U32},
    {"u64", U64},
};

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
    // if it is in keywords, push it o
    if (keywords.find(word) != keywords.end())
    {
        tokens.push_back(Token(keywords[word], word, line));
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
    if (keywords.find(type) != keywords.end())
    {
        tokens.push_back(Token(keywords[type], type, line));
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

        // two char tokens
        else if (curr == '!' || curr == '<' || curr == '>' || curr == '=') 
        {
            std::string twoCharToken = std::string(1, curr);
            advance();
            twoCharToken += std::string(1, currentChar());
            if (mulicharTokens.find(twoCharToken) != mulicharTokens.end())
            {
                tokens.push_back(Token(mulicharTokens[twoCharToken], twoCharToken, line));
            }
            else
            {
                tokens.push_back(Token(singleCharTokens[std::string(1, curr)], std::string(1, curr), line));
            }
            advance();
        }
        else if (singleCharTokens.find(std::string(1, curr)) != singleCharTokens.end())
        {
            tokens.push_back(Token(singleCharTokens[std::string(1, curr)], std::string(1, curr), line));
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