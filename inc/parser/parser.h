#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "astNode.h"
#include "lexer.h"

class Parser {
public:
    Parser(const std::vector<Token>& tokens);

    ASTNodePtr parse();

private:
    std::vector<Token> tokens;
    int current = 0;

    bool match(TokenType type);
    const Token peek() const;
    void advance();
    ASTNodePtr parseExpression();
    ASTNodePtr parseTerm();
    ASTNodePtr parseFactor();
};

#endif // PARSER_H