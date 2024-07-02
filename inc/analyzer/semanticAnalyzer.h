#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "astVisitor.h"
#include "symbolTable.h"

class SemanticAnalyzer : public ASTVisitor {
public:
    SemanticAnalyzer() {
        symbolTable = SymbolTable();
    }
    ~SemanticAnalyzer() = default;

    void analyze(ASTNodePtr root);

    void visit(NumberNode& node) override;
    void visit(IdentifierNode& node) override;
    void visit(BinaryOperatorNode& node) override;
    void visit(EqualsNode& node) override;
    void visit(LetNode& node) override;
    void visit(FunctionNode& node) override;
    void visit(BooleanLiteralNode& node) override;
    // Implement other visit methods...

private:
    SymbolTable symbolTable;

    void reportError(const std::string& message);
};

#endif // SEMANTIC_ANALYZER_H