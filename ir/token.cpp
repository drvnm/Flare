#include "token.hpp"
#include <iostream>

void Token::print() {
    std::cout << "Token(" << type << ", " << lexeme << ", " << line << ") ";
}
