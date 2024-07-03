#include "parser.h"
#include <stdexcept>
#include <iostream>

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
    if(match(TokenType::Let)) {
        auto identifier = parseFactor();
        if (!match(TokenType::Equals)) {
            throw std::runtime_error("Expected '='");
        }
        auto value = parseExpression();
        return std::make_shared<LetNode>(identifier, value);
    }
    else if (match(TokenType::Keyword) && tokens[current-1].value == "def") {
        // Start of function
        if (!match(TokenType::Identifier)) {
            throw std::runtime_error("Expected function name");
        }
        std::string name = tokens[current-1].value;

        // Handle function arguments
        std::vector<ASTNodePtr> params;
        if (!match(TokenType::OpenParen)) {
            throw std::runtime_error("Expected '('");
        }
        while (!match(TokenType::ClosedParen)) {
            if (!match(TokenType::Identifier)) {
                throw std::runtime_error("Expected parameter name");
            }
            params.push_back(std::make_shared<IdentifierNode>(tokens[current-1].value));
        }

        // Handle function body
        std::vector<ASTNodePtr> bodyNodes;
        if (!match(TokenType::OpenBracket)) {
            throw std::runtime_error("Expected '{'");
        }
        while (!match(TokenType::ClosedBracket)) {
            bodyNodes.push_back(parseExpression());
        }
        if (bodyNodes.empty()) {
            throw std::runtime_error("Expected function body");
        }
        return std::make_shared<FunctionNode>(name, params, bodyNodes);
    }

    return parseTerm();
}

ASTNodePtr Parser::parseTerm() {
    auto node = parseFactor();
    if(match(TokenType::Add) || match(TokenType::Subtract) || match(TokenType::Multiply) || match(TokenType::Divide)) {
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
    else if (match(TokenType::BooleanLiteral)) {
        bool value = tokens[current-1].value == "True";
        return std::make_shared<BooleanLiteralNode>(value);
    }
    else if (match(TokenType::OpenParen)) {
        ASTNodePtr node = parseExpression();
        if (!match(TokenType::ClosedParen)) {
            throw std::runtime_error("Expected ')'");
        }
        return node;
    }
    else {
        throw std::runtime_error("Unexpected token " + tokens[current].value);
    }
}