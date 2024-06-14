#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>
#include <memory>
#include "lexer.h"

class ASTNode;
using ASTNodePtr = std::shared_ptr<ASTNode>;

class ASTNode {
public:
    ASTNode(TokenType type);
    virtual ~ASTNode() = default;

    TokenType getType();
    void addChild(ASTNodePtr child);
    const std::vector<ASTNodePtr>& getChildren();

private:
    TokenType type;
    std::vector<ASTNodePtr> children;
};

class NumberNode : public ASTNode {
public:
    NumberNode(int value);
    int getValue() const;

private:
    int value;
};

class IdentifierNode : public ASTNode {
public:
    IdentifierNode(const std::string& name);
    const std::string& getName() const;

private:
    std::string name;
};

class BinaryOperatorNode : public ASTNode {
public:
    BinaryOperatorNode(TokenType op, ASTNodePtr left, ASTNodePtr right);
    ASTNodePtr getLeft() const;
    ASTNodePtr getRight() const;

private:
    ASTNodePtr left;
    ASTNodePtr right;
};

class EqualsNode : public ASTNode {
public:
    Equals(ASTNodePtr left, ASTNodePtr right);
    ASTNodePtr getLeft() const;
    ASTNodePtr getRight() const;

private:
    ASTNodePtr left;
    ASTNodePtr right;
};

class LetNode : public : ASTNode {
public:
    LetNode(ASTNodePtr identifier, ASTNodePtr value);
    ASTNodePtr getIdentifier() const;
    ASTNodePtr getValue() const;
private:
    ASTNodePtr identifier;
    ASTNodePtr value;
};


#endif // ASTNODE_H