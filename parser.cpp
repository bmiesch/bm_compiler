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
    std::vector<ASTNodePtr> children;
};


class NumberNode : public ASTNode {
public:
    NumberNode(int value) : ASTNode(TokenType::Number), value(value) {}

    int getValue() const {return value;}

private:
    int value;
};


class IdentifierNode : public ASTNode {
public:
    IdentifierNode(const std::string& name) : ASTNode(TokenType::Identifier), name(name) {}

    const std::string& getName() const {return name;}

private:
    std::string name;
};


class Parser {
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    // Entry function
    ASTNodePtr parse() {
        // Start parsing and return the root of the AST
        return nullptr;
    }

private:
    std::vector<Token> tokens;
    int current = 0;

    bool match(TokenType type) {
        if(peek().type == type){
            advance();
            return true;
        }
        return false;
    }
    
    const Token& peek() const {
        if (current < tokens.size()) {return tokens[current];}

        // At the end of tokens
        Token endToken = Token{"", TokenType::End};
        return endToken;
    }

    void advance() {
        if (current < tokens.size()) {current++;}
    }

    ASTNodePtr parseExpression() {
        auto node = parseTerm();
        // TODO: Implement
    }
    ASTNodePtr parseTerm() {
        auto node = parseFactor();
        // TODO: Implement
    }
    ASTNodePtr parseFactor() {
        if(match(TokenType::Number)){
            return std::make_shared<NumberNode>(std::stoi(peek().value));
        }
        else if (match(TokenType::Identifier)) {
            return std::make_shared<IdentifierNode>(peek().value);
        }
    }
    void parseLetStatement();
    void parseFunctionDefinition();


};