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
    if(match(TokenType::Let)) {
        auto identifier = parseFactor();
        if (!match(TokenType::Equals)) {
            throw std::runtime_error("Expected '='");
        }
        auto value = parseExpression();
        node = std::make_shared<LetNode>(identifier, value);
    }

    return node;
}

ASTNodePtr Parser::parseTerm() {
    auto node = parseFactor();
    if(match(TokenType::BinaryOperator)) {
        auto op = tokens[current-1].type;
        auto right = parseTerm();
        node = std::make_shared<BinaryOperatorNode>(op, node, right);
    }
    else if (match(TokenType::Equals)) {
        auto right = parseExpression();
        node = std::make_shared<EqualsNode>(node, right);
    }

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
    else if (match(TokenType::OpenParen)) {
        ASTNodePtr node = parseExpression();
        if (!match(TokenType::ClosedParen)) {
            throw std::runtime_error("Expected ')'");
        }
        return node;
    }
    else {
        throw std::runtime_error("Unexpected token");
    }
}