#include <iostream>
#include "parser.h"

int main() {
    std::vector<Token> tokens = {
            { "3", TokenType::Number },
            { "+", TokenType::BinaryOperator },
            { "4", TokenType::Number },
            { "*", TokenType::BinaryOperator },
            { "2", TokenType::Number },
            { "", TokenType::End }
    };

    Parser parser(tokens);
    try {
        ASTNodePtr ast = parser.parse();
        std::cout << "Parsing successful!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}