#include "astNode.h"
#include "astVisitor.h"


// NumberNode implementation
void NumberNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// IdentifierNode implementation
void IdentifierNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// BinaryOperatorNode implementation
void BinaryOperatorNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// EqualsNode implementation
void EqualsNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// LetNode implementation
void LetNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// FunctionNode implementation
void FunctionNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

// BooleanLiteralNode implementation
void BooleanLiteralNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}
