#include "astNode.h"

ASTNode::ASTNode(TokenType type) : type(type) {}

TokenType ASTNode::getType() { return type; }

void ASTNode::addChild(ASTNodePtr child){
    children.push_back(child);
}

const std::vector<ASTNodePtr>& ASTNode::getChildren() {
    return children;
}

NumberNode::NumberNode(int value) : ASTNode(TokenType::Number), value(value) {}

int NumberNode::getValue() const { return value; }

IdentifierNode::IdentifierNode(const std::string& name) : ASTNode(TokenType::Identifier), name(name) {}

const std::string& IdentifierNode::getName() const { return name; }

BinaryOperatorNode::BinaryOperatorNode(TokenType op, ASTNodePtr left, ASTNodePtr right)
        : ASTNode(op), left(left), right(right) {}

ASTNodePtr BinaryOperatorNode::getLeft() const { return left; }

ASTNodePtr BinaryOperatorNode::getRight() const { return right; }

Equals::Equals(ASTNodePtr left, ASTNodePtr right) : ASTNode(TokenType::Equals), left(left), right(right) {}

ASTNodePtr Equals::getLeft() const { return left; }

ASTNodePtr Equals::getRight() const { return right; }

LetNode::LetNode(ASTNodePtr identifier, ASTNodePtr value) : ASTNode(TokenType::Let), identifier(identifier), value(value) {}

ASTNodePtr LetNode::getIdentifier() const { return identifier; }

ASTNodePtr LetNode::getValue() const { return value; }
