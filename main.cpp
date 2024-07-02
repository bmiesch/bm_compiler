#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"
#include "semanticAnalyzer.h"


void generateDotFile(const ASTNodePtr& root, const std::string& filename) {
    std::ofstream file(filename);
    file << "digraph G {\n";
    file << root->toDot();
    file << "}\n";
    file.close();
}


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
    std::cout << "Tokenization successful!" << std::endl;

    Parser parser(tokens);
    ASTNodePtr ast;
    try {
        ast = parser.parse();
        std::cout << "Parsing successful!" << std::endl;
        generateDotFile(ast, "ast.dot");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    SemanticAnalyzer semanticAnalyzer;
    ast->accept(semanticAnalyzer);

    return 0;
}