#include "semanticAnalyzer.h"
#include <iostream>


void SemanticAnalyzer::analyze(ASTNodePtr root) {
    root->accept(*this);
}

void SemanticAnalyzer::visit(NumberNode& node) {
    // Nothing for now
}

void SemanticAnalyzer::visit(IdentifierNode& node) {
    SymbolInfo symbolInfo;
    if (!symbolTable.lookup(node.getName(), symbolInfo)) {
        std::cerr << "Error: Identifier " << node.getName() << " not found" << std::endl;
    }
}

void SemanticAnalyzer::visit(BinaryOperatorNode& node) {
    node.getLeft()->accept(*this);
    node.getRight()->accept(*this);

    if (node.getLeft()->getNodeType() != ASTNodeType::Number && node.getLeft()->getNodeType() != ASTNodeType::Identifier) {
        reportError("Left side of binary operator is not a number or identifier");
    }
    if (node.getRight()->getNodeType() != ASTNodeType::Number && node.getRight()->getNodeType() != ASTNodeType::Identifier) {
        reportError("Right side of binary operator is not a number or identifier");
    }
}

void SemanticAnalyzer::visit(EqualsNode& node) {
    node.getLeft()->accept(*this);
    node.getRight()->accept(*this);

    if (node.getLeft()->getNodeType() != ASTNodeType::Identifier) {
        reportError("Left side of assignment must be an identifier");
    }
}

void SemanticAnalyzer::visit(LetNode& node) {
    auto identifierNode = std::dynamic_pointer_cast<IdentifierNode>(node.getIdentifier());
    if (!identifierNode) {
        reportError("Left side of let statement must be an identifier");
        return;
    }

    // Determine the data type of the value
    std::string dataType;
    if (node.getValue()->getNodeType() == ASTNodeType::Number) {
        dataType = "int";
    } else if (node.getValue()->getNodeType() == ASTNodeType::Boolean) {
        dataType = "bool";
    } else if (node.getValue()->getNodeType() == ASTNodeType::Identifier) {
        node.getValue()->accept(*this);
        dataType = "unknown";
    } else if (node.getValue()->getNodeType() == ASTNodeType::BinaryExpression) {
        dataType = "unknown";
    } else {
        reportError("Unsupported data type in let statement");
        return;
    }

    symbolTable.insert(identifierNode->getName(), SymbolType::VARIABLE, dataType);

    node.getValue()->accept(*this);
}

void SemanticAnalyzer::visit(FunctionNode& node) {
    symbolTable.enterScope();

    for (const auto& param : node.getParams()) {
        param->accept(*this);

        // Params can only be identifiers
        if (param->getNodeType() != ASTNodeType::Identifier) {
            reportError("Function parameters must be identifiers");
            return;
        }

        if (param->getNodeType() == ASTNodeType::Identifier) {
            auto identifierNode = std::dynamic_pointer_cast<IdentifierNode>(param);
            symbolTable.insert(identifierNode->getName(), SymbolType::PARAMETER, "int");
        }
    }

    for (const auto& bodyNode : node.getBodyNodes()) {
        bodyNode->accept(*this);
    }

    symbolTable.exitScope();
}

void SemanticAnalyzer::visit(BooleanLiteralNode& node) {
    // Nothing for now
}

void SemanticAnalyzer::reportError(const std::string& errorMessage) {
    std::cerr << "Error: " << errorMessage << std::endl;
}