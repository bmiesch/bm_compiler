#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "astNode.h"
#include "symbolTable.h"

class SemanticAnalyzer {
public:
    SemanticAnalyzer();
    ~SemanticAnalyzer();

    void analyze(ASTNodePtr root);

private:
    SymbolTable symbolTable;

    void visitNode(ASTNodePtr node);
    void visitProgram(ASTNodePtr node);
    void visitVariableDeclaration(ASTNodePtr node);
    void visitFunctionDeclaration(ASTNodePtr node);
    void visitFunctionCall(ASTNodePtr node);
    void visitAssignment(ASTNodePtr node);
    void visitBinaryExpression(ASTNodePtr node);
    void visitUnaryExpression(ASTNodePtr node);
    void visitIdentifier(ASTNodePtr node);
    void visitNumber(ASTNodePtr node);
    void visitBoolean(ASTNodePtr node);
    void visitIfStatement(ASTNodePtr node);
    void visitWhileStatement(ASTNodePtr node);
    void visitReturnStatement(ASTNodePtr node);
    // Add more visit methods for other AST node types

    void checkType(ASTNodePtr node, const std::string& expectedType);
    void checkIdentifierUsage(ASTNodePtr node);
    void checkFunctionCall(ASTNodePtr node);
    // Add more semantic checking methods as needed

    void reportError(const std::string& message, ASTNodePtr node);
};

#endif