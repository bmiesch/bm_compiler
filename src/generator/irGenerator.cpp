#include "irGenerator.h"

void IRGenerator::generateIR(ASTNodePtr root) {
    root->accept(*this);
}

void IRGenerator::generateInstruction(IRInstructionType type, const std::string& dest, const std::string& src1, const std::string& src2) {
    irInstructions.emplace_back(type, dest, src1, src2);
}

std::string IRGenerator::handleLiteral(ASTNodePtr node) {
    if (auto numberNode = std::dynamic_pointer_cast<NumberNode>(node)) {
        std::string tempVar = newTempVar();
        generateInstruction(IRInstructionType::LOAD, tempVar, std::to_string(numberNode->getValue()), "");
        return tempVar;
    } else if (auto booleanNode = std::dynamic_pointer_cast<BooleanLiteralNode>(node)) {
        std::string tempVar = newTempVar();
        generateInstruction(IRInstructionType::LOAD, tempVar, booleanNode->getValue() ? "true" : "false", "");
        return tempVar;
    } else if (auto identifierNode = std::dynamic_pointer_cast<IdentifierNode>(node)) {
        std::string tempVar = newTempVar();
        generateInstruction(IRInstructionType::LOAD, tempVar, identifierNode->getName(), "");
        return tempVar;
    }
    return "";
}

void IRGenerator::visit(BinaryOperatorNode& node) {
    auto left = node.getLeft();
    auto right = node.getRight();

    if (left && right) {
        std::string leftTemp = handleLiteral(left);
        std::string rightTemp = handleLiteral(right);

        std::string resultTemp = newTempVar();
        IRInstructionType instrType;

        // Determine the instruction type based on the operator
        switch (node.getTokenType()) {
            case TokenType::Add:
                instrType = IRInstructionType::ADD;
                break;
            case TokenType::Subtract:
                instrType = IRInstructionType::SUB;
                break;
            case TokenType::Multiply:
                instrType = IRInstructionType::MUL;
                break;
            case TokenType::Divide:
                instrType = IRInstructionType::DIV;
                break;
            default:
                throw std::runtime_error("Unsupported binary operator");
        }

        generateInstruction(instrType, resultTemp, leftTemp, rightTemp);

        // Store the result in the node
        node.setResultVar(resultTemp);
    }
}

void IRGenerator::visit(LetNode& node) {
    auto identifier = std::dynamic_pointer_cast<IdentifierNode>(node.getIdentifier());
    auto value = node.getValue();

    if (identifier && value) {
        std::string valueTemp;
        if (auto binaryOpNode = std::dynamic_pointer_cast<BinaryOperatorNode>(value)) {
            // Visit the binary operation node to generate the instructions
            binaryOpNode->accept(*this);
            valueTemp = binaryOpNode->getResultVar();
        } else {
            valueTemp = handleLiteral(value);
        }

        generateInstruction(IRInstructionType::ALLOCA, identifier->getName(), "", "");
        generateInstruction(IRInstructionType::STORE, identifier->getName(), valueTemp, "");
    }
}

void IRGenerator::visit(FunctionNode& node) {
    for (const auto& bodyNode : node.getBodyNodes()) {
        bodyNode->accept(*this);
    }
    generateInstruction(IRInstructionType::RET, "", "", "");
}

void IRGenerator::visit(NumberNode& node) {
    generateInstruction(IRInstructionType::LOAD, newTempVar(), std::to_string(node.getValue()), "");
}

void IRGenerator::visit(IdentifierNode& node) {
    generateInstruction(IRInstructionType::LOAD, newTempVar(), node.getName(), "");
}

void IRGenerator::visit(BooleanLiteralNode& node) {
    generateInstruction(IRInstructionType::LOAD, newTempVar(), node.getValue() ? "true" : "false", "");
}
