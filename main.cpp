#include <string>
#include <fstream>
#include <iostream>

#include "ir/lexer.hpp"
#include "parser/parser.hpp"
#include "visitors/compiler.hpp"

int main()
{
    // read file main.flr
    std::ifstream file("main.flr");
    std::string source((std::istreambuf_iterator<char>(file)),
                       (std::istreambuf_iterator<char>()));

    std::cout << "Source: " << source << std::endl;

    Lexer lexer = Lexer(source);
    lexer.lex();

    Parser parser = Parser(lexer.tokens);
    parser.program();

    Compiler().compile(parser.statements);
}