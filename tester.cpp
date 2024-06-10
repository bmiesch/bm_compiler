#include <iostream>
#include <vector>
#include "lexer.cpp" 

void printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << "Token: " << token.value << ", Type: " << token.type << std::endl;
    }
}

int main() {
    std::string sourceCode = "let x = 42 ( )";
    std::vector<Token> tokens = tokenize(sourceCode);

    printTokens(tokens);

    return 0;
}