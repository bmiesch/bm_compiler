
#include "lexer.cpp"


class Parser {
private:
    std::vector<Token> tokens;
    int currrent = 0;

public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    ASTNode* parse() {
        // Start parsing and return the root of the AST
        return nullptr;
    }
};