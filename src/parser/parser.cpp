#include "parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

ASTNodePtr Parser::parse() {
    return parseExpression();
}

bool Parser::match(TokenType type) {
    if(peek().type == type){
        advance();
        return true;
    }
    return false;
}

const Token Parser::peek() const {
    if (current < tokens.size()) {return tokens[current];}

    Token endToken = Token{"", TokenType::End};
    return endToken;
}

void Parser::advance() {
    if (current < tokens.size()) {current++;}
}

ASTNodePtr Parser::parseExpression() {
    auto node = parseTerm();
    if(match(TokenType::BinaryOperator)) {
        auto op = tokens[current-1].type;
        auto right = parseTerm();
        node = std::make_shared<BinaryOperatorNode>(op, node, right);
    }
    return node;
}

ASTNodePtr Parser::parseTerm() {
    auto node = parseFactor();
    return node;
}

ASTNodePtr Parser::parseFactor() {
    if(match(TokenType::Number)){
        int value = std::stoi(tokens[current-1].value);
        return std::make_shared<NumberNode>(value);
    }
    else if (match(TokenType::Identifier)) {
        return std::make_shared<IdentifierNode>(tokens[current-1].value);
    }
    else {
        throw std::runtime_error("Unexpected token");
    }
}