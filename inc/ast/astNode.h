#ifndef ASTNODE_H
#define ASTNODE_H

#include <vector>
#include <memory>
#include "lexer.h"

enum class ASTNodeType {
    VariableDeclaration,
    FunctionDeclaration,
    BinaryExpression,
    Identifier,
    Number,
    Boolean,
    // Add more AST node types as needed
};

class ASTNode;
using ASTNodePtr = std::shared_ptr<ASTNode>;

class ASTVisitor;

class ASTNode {
public:
    ASTNode(TokenType tokenType, ASTNodeType nodeType)
        : tokenType(tokenType), nodeType(nodeType) {}
    virtual ~ASTNode() = default;

    TokenType getTokenType() const { return tokenType; }
    ASTNodeType getNodeType() const { return nodeType; }

    void addChild(ASTNodePtr child) { children.push_back(child); }
    const std::vector<ASTNodePtr>& getChildren() const { return children; }

    virtual void accept(ASTVisitor& visitor) = 0;

    // Function to generate a dot file for the AST
    virtual std::string toDot() const {
        std::string dot = "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " [label=\"" + getTypeName(tokenType) + "\"];\n";
        for (const auto& child : children) {
            if (child) { // Check if the child pointer is not null
                dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(child.get())) + ";\n";
                dot += child->toDot();
            }
        }
        return dot;
    }

    // Dummy function for token type name
    std::string getTypeName(TokenType tokenType) const {
        switch (tokenType) {
            case TokenType::Number: return "Number";
            case TokenType::Identifier: return "Identifier";
            case TokenType::Equals: return "Equals";
            case TokenType::OpenParen: return "OpenParen";
            case TokenType::ClosedParen: return "ClosedParen";
            case TokenType::OpenBracket: return "OpenBracket";
            case TokenType::ClosedBracket: return "ClosedBracket";
            case TokenType::BinaryOperator: return "BinaryOperator";
            case TokenType::Let: return "Let";
            case TokenType::Keyword: return "Keyword";
            case TokenType::BooleanLiteral: return "BooleanLiteral";
            case TokenType::End: return "End";
        }
        return "";
    }

protected:
    TokenType tokenType;
    ASTNodeType nodeType;
    std::vector<ASTNodePtr> children;
};

class NumberNode : public ASTNode {
public:
    NumberNode(int value)
        : ASTNode(TokenType::Number, ASTNodeType::Number), value(value) {}

    int getValue() const { return value; }

    void accept(ASTVisitor& visitor) override;

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
    IdentifierNode(const std::string& name)
        : ASTNode(TokenType::Identifier, ASTNodeType::Identifier), name(name) {}

    const std::string& getName() const { return name; }

    void accept(ASTVisitor& visitor) override;

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
    BinaryOperatorNode(TokenType op, ASTNodePtr left, ASTNodePtr right)
        : ASTNode(op, ASTNodeType::BinaryExpression), left(left), right(right) {}

    ASTNodePtr getLeft() const { return left; }
    ASTNodePtr getRight() const { return right; }

    void accept(ASTVisitor& visitor) override;

private:
    ASTNodePtr left;
    ASTNodePtr right;

    // Override toDot to include the operator
    std::string toDot() const override {
        std::string dot = "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " [label=\"" + getTypeName(tokenType) + "\"];\n";
        dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(left.get())) + ";\n";
        dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(right.get())) + ";\n";
        return dot + left->toDot() + right->toDot();
    }
};

class EqualsNode : public ASTNode {
public:
    EqualsNode(ASTNodePtr left, ASTNodePtr right)
        : ASTNode(TokenType::Equals, ASTNodeType::BinaryExpression), left(left), right(right) {}

    ASTNodePtr getLeft() const { return left; }
    ASTNodePtr getRight() const { return right; }

    void accept(ASTVisitor& visitor) override;

private:
    ASTNodePtr left;
    ASTNodePtr right;

    // Override toDot to include the operator
    std::string toDot() const override {
        std::string dot = "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " [label=\"" + getTypeName(tokenType) + "\"];\n";
        dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(left.get())) + ";\n";
        dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(right.get())) + ";\n";
        return dot + left->toDot() + right->toDot();
    }
};

class LetNode : public ASTNode {
public:
    LetNode(ASTNodePtr identifier, ASTNodePtr value)
        : ASTNode(TokenType::Let, ASTNodeType::VariableDeclaration), identifier(identifier), value(value) {}

    ASTNodePtr getIdentifier() const { return identifier; }
    ASTNodePtr getValue() const { return value; }

    void accept(ASTVisitor& visitor) override;

private:
    ASTNodePtr identifier;
    ASTNodePtr value;

    // Override toDot to include the operator
    std::string toDot() const override {
        std::string dot = "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " [label=\"" + getTypeName(tokenType) + "\"];\n";
        dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(identifier.get())) + ";\n";
        dot += "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " -> node" + std::to_string(reinterpret_cast<std::uintptr_t>(value.get())) + ";\n";
        return dot + identifier->toDot() + value->toDot();
    }
};

class FunctionNode : public ASTNode {
public:
    FunctionNode(const std::string& name, const std::vector<ASTNodePtr>& params, const std::vector<ASTNodePtr>& bodyNodes)
        : ASTNode(TokenType::Keyword, ASTNodeType::FunctionDeclaration), name(name), params(params), bodyNodes(bodyNodes) {}

    const std::string& getName() const { return name; }
    const std::vector<ASTNodePtr>& getParams() const { return params; }
    const std::vector<ASTNodePtr>& getBodyNodes() const { return bodyNodes; }

    void accept(ASTVisitor& visitor) override;

private:
    std::string name;
    std::vector<ASTNodePtr> params;
    std::vector<ASTNodePtr> bodyNodes;

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
};

class BooleanLiteralNode : public ASTNode {
public:
    BooleanLiteralNode(bool value)
        : ASTNode(TokenType::BooleanLiteral, ASTNodeType::Boolean), value(value) {}

    bool getValue() const { return value; }

    void accept(ASTVisitor& visitor) override;

private:
    bool value;

    // Override toDot to include the value
    std::string toDot() const override {
        std::string dot = "node" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) + " [label=\"" + (value ? "true" : "false") + "\"];\n";
        return dot + ASTNode::toDot();
    }
};

#endif // ASTNODE_H