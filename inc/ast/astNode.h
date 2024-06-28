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


    // Function to generate a dot file for the AST
    virtual std::string toDot() const {
        std::string dot = "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " [label=\"" + getTypeName(type) + "\"];\n";
        for (const auto& child : children) {
            if (child) { // Check if the child pointer is not null
                dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(child.get())) + ";\n";
                dot += child->toDot();
            }
        }
        return dot;
    }

    // Dummy function for token type name
    std::string getTypeName(TokenType type) const {
        switch (type) {
            case TokenType::Number: return "Number";
            case TokenType::Identifier: return "Identifier";
            case TokenType::Equals: return "Equals";
            case TokenType::OpenParen: return "OpenParen";
            case TokenType::ClosedParen: return "ClosedParen";
            case TokenType::BinaryOperator: return "BinaryOperator";
            case TokenType::Let: return "Let";
            case TokenType::Keyword: return "Keyword";
            case TokenType::BooleanLiteral: return "BooleanLiteral";
            case TokenType::End: return "End";
        }
        return "";
    }

protected:
    TokenType type;
    std::vector<ASTNodePtr> children;
};

class NumberNode : public ASTNode {
public:
    NumberNode(int value);
    int getValue() const;

private:
    int value;

    // Override toDot to include the value
    std::string toDot() const override {
        std::string dot = "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " [label=\"" + std::to_string(value) + "\"];\n";
        return dot + ASTNode::toDot();
    }
};

class IdentifierNode : public ASTNode {
public:
    IdentifierNode(const std::string& name);
    const std::string& getName() const;

private:
    std::string name;

    // Override toDot to include the name
    std::string toDot() const override {
        std::string dot = "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " [label=\"" + name + "\"];\n";
        return dot + ASTNode::toDot();
    }
};

class BinaryOperatorNode : public ASTNode {
public:
    BinaryOperatorNode(TokenType op, ASTNodePtr left, ASTNodePtr right);
    ASTNodePtr getLeft() const;
    ASTNodePtr getRight() const;

private:
    ASTNodePtr left;
    ASTNodePtr right;

    // Override toDot to include the operator
    std::string toDot() const override {
        std::string dot = "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " [label=\"" + getTypeName(type) + "\"];\n";
        dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(left.get())) + ";\n";
        dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(right.get())) + ";\n";
        return dot + left->toDot() + right->toDot();
    }
};

class EqualsNode : public ASTNode {
public:
    EqualsNode(ASTNodePtr left, ASTNodePtr right);
    ASTNodePtr getLeft() const;
    ASTNodePtr getRight() const;

private:
    ASTNodePtr left;
    ASTNodePtr right;

    // Override toDot to include the operator
    std::string toDot() const override {
        std::string dot = "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " [label=\"" + getTypeName(type) + "\"];\n";
        dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(left.get())) + ";\n";
        dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(right.get())) + ";\n";
        return dot + left->toDot() + right->toDot();
    }
};

class LetNode : public ASTNode {
public:
    LetNode(ASTNodePtr identifier, ASTNodePtr value);
    ASTNodePtr getIdentifier() const;
    ASTNodePtr getValue() const;
private:
    ASTNodePtr identifier;
    ASTNodePtr value;

    // Override toDot to include the operator
    std::string toDot() const override {
        std::string dot = "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " [label=\"" + getTypeName(type) + "\"];\n";
        dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(identifier.get())) + ";\n";
        dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(value.get())) + ";\n";
        return dot + identifier->toDot() + value->toDot();
    }
};

class FunctionNode : public ASTNode {
public:
    FunctionNode(const std::string& name, const std::vector<ASTNodePtr>& params, const std::vector<ASTNodePtr>& bodyNodes)
            : ASTNode(TokenType::Keyword), name(name), params(params), bodyNodes(bodyNodes) {}

    const std::string& getName() const {
        return name;
    }

    const std::vector<ASTNodePtr>& getParams() const {
        return params;
    }

    const std::vector<ASTNodePtr>& getBodyNodes() const {
        return bodyNodes;
    }

    std::string toDot() const override {
        std::string dot = "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " [label=\"" + name + "\"];\n";
        for (const auto& param : params) {
            dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(param.get())) + ";\n";
            dot += param->toDot();
        }
        for (const auto& bodyNode : bodyNodes) {
            dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(bodyNode.get())) + ";\n";
            dot += bodyNode->toDot();
        }
        return dot;
    }

private:
    std::string name;
    std::vector<ASTNodePtr> params;
    std::vector<ASTNodePtr> bodyNodes;
};

class BooleanLiteralNode : public ASTNode {
public:
    BooleanLiteralNode(bool value);
    bool getValue() const;

private:
    bool value;

    // Override toDot to include the value
    std::string toDot() const override {
        std::string dot = "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " [label=\"" + (value ? "true" : "false") + "\"];\n";
        return dot + ASTNode::toDot();
    }
};

#endif // ASTNODE_H