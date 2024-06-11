#include <vector>
#include <memory>
#include "lexer.cpp"

// Forward declaration of the ASTNode class
class ASTNode;

// Define a type for a list of ASTNode shared pointers
using ASTNodePtr = std::shared_ptr<ASTNode>;

class ASTNode {
public:
    ASTNode(TokenType type) : type(type) {}
    virtual ~ASTNode() = default;

    TokenType getType() {return type;}

    void addChild(ASTNodePtr child){
        children.push_back(child);
    }

    const std::vector<ASTNodePtr>& getChildren() {
        return children;
    }

private:
    TokenType type;
    std::vector<ASTNodePtr> children = 0;
};

class NumberNode : public ASTNode {
public:
    NumberNode() : ASTNode(TokenType::Number) {}
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    ASTNode* parse() {
        // Start parsing and return the root of the AST
        return nullptr;
    }

private:
    std::vector<Token> tokens;
    int current = 0;
};