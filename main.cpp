#include <iostream>
#include <string>

#include "ir/lexer.hpp"
#include "parser/parser.hpp"
#include "visitors/compiler.hpp"

int main() {
    std::string source;
    std::cout << "> ";
    std::getline(std::cin, source);


    Lexer lexer = Lexer(source);
    lexer.lex();

    for (Token token : lexer.tokens) {
        std::cout << token.type << " " << token.lexeme << std::endl;
    }
 

    Parser parser = Parser(lexer.tokens);
    parser.program();


    Compiler().compile(parser.statements);


    std::cout << "\nfinsihed" << std::endl;

}