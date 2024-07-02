#ifndef ASTVISITOR_H
#define ASTVISITOR_H

#include "astNode.h"

class ASTVisitor {
public:
    virtual void visit(NumberNode& node) = 0;
    virtual void visit(IdentifierNode& node) = 0;
    virtual void visit(BinaryOperatorNode& node) = 0;
    virtual void visit(EqualsNode& node) = 0;
    virtual void visit(LetNode& node) = 0;
    virtual void visit(FunctionNode& node) = 0;
    virtual void visit(BooleanLiteralNode& node) = 0;
    // Add visit methods for other AST node types...
};

#endif // ASTVISITOR_H