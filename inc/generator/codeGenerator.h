#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "irGenerator.h"
#include <iostream>
#include <iomanip>

class CodeGenerator {
public:
    CodeGenerator() = default;
    ~CodeGenerator() = default;

    void generateCode(const std::vector<IRInstruction>& instructions);

    void disassembleCode() const;

    void printCode() const {
        for (uint8_t byte : generatedCode) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << ' ';
        }
        std::cout << std::endl;
    }

private:
    void handleAdd(const IRInstruction& instruction);
    void handleSub(const IRInstruction& instruction);
    void handleMul(const IRInstruction& instruction);
    void handleDiv(const IRInstruction& instruction);
    void handleLoad(const IRInstruction& instruction);
    void handleStore(const IRInstruction& instruction);
    void handleAlloc(const IRInstruction& instruction);
    void handleRet(const IRInstruction& instruction);

    std::vector<uint8_t> generatedCode;

    std::unordered_map<std::string, uint8_t> registerMap;
    uint8_t nextRegister = 0;

    void encodeInstruction(uint8_t opcode);
    void encodeInstruction(uint8_t opcode, uint8_t reg, uint8_t rm);
    void encodeInstruction(uint8_t opcode, uint8_t opcode2, uint8_t reg, uint8_t rm);

    void encodeImmediate(uint8_t opcode, uint8_t reg, int32_t imm);

    uint8_t getRegisterCode(const std::string& reg);
    uint8_t allocateRegister(const std::string& var);
    void spillRegister();
    void reloadRegister(const std::string& var);

    bool isInteger(const std::string& s);
};

#endif // CODE_GENERATOR_H