#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "astNode.h"
#include "astVisitor.h"
#include <vector>
#include <string>
#include <iostream>

// Enum for different types of IR instructions
enum class IRInstructionType {
    ADD,
    SUB,
    MUL,
    DIV,
    LOAD,
    STORE,
    ALLOCA,
    RET,
    // Add more as needed
};

// Structure to represent an IR instruction
struct IRInstruction {
    IRInstructionType type;
    std::string dest;
    std::string src1;
    std::string src2;

    IRInstruction(IRInstructionType t, const std::string& d, const std::string& s1, const std::string& s2 = "")
        : type(t), dest(d), src1(s1), src2(s2) {}
};

class IRGenerator : public ASTVisitor {
public:
    IRGenerator() : tempVarCounter(0) {}
    ~IRGenerator() = default;

    void generateIR(ASTNodePtr root);

    void printIR() {
        for (const auto& instr : irInstructions) {
            std::cout << "Instruction: " << static_cast<int>(instr.type) << " " << instr.dest << " " << instr.src1 << " " << instr.src2 << std::endl;
        }
    }

    // Visitor methods
    void visit(BinaryOperatorNode& node) override;
    void visit(LetNode& node) override;
    void visit(FunctionNode& node) override;

    void visit(NumberNode& node) override;
    void visit(IdentifierNode& node) override;
    void visit(BooleanLiteralNode& node) override;
    void visit(EqualsNode& node) override {}

private:
    std::vector<IRInstruction> irInstructions;
    int tempVarCounter;

    std::string newTempVar() { return "t" + std::to_string(tempVarCounter++); }

    void generateInstruction(IRInstructionType type, const std::string& dest, const std::string& src1, const std::string& src2 = "");

    std::string handleLiteral(ASTNodePtr node);
};

#endif // IR_GENERATOR_H