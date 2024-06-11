#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.cpp"


std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Could not open file");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << "Token: " << token.value << ", Type: " << token.type << std::endl;
    }
}


int main(int argc, char* argv[]) {

    if(argc != 2) {
        std::cerr << "Incorrect usage" << std::endl;
        std::cerr << "Usage: ./main <input file>" << std::endl;
        return 1;
    }

    std::string sourceCode = readFile(argv[1]);
    std::vector<Token> tokens = tokenize(sourceCode);

    printTokens(tokens);

    return 0;
}