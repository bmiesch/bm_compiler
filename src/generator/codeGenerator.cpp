#include "codeGenerator.h"
#include <capstone/capstone.h>
#include <iostream>
#include <vector>
#include <iomanip>


void CodeGenerator::generateCode(const std::vector<IRInstruction>& instructions) {
    for (const auto& instruction : instructions) {
        switch(instruction.type) {
            case IRInstructionType::ADD:
                handleAdd(instruction);
                break;
            case IRInstructionType::SUB:
                handleSub(instruction);
                break;
            case IRInstructionType::MUL:
                handleMul(instruction);
                break;
            case IRInstructionType::DIV:
                handleDiv(instruction);
                break;
            case IRInstructionType::LOAD:
                handleLoad(instruction);
                break;
            case IRInstructionType::STORE:
                handleStore(instruction);
                break;
            case IRInstructionType::ALLOC:
                handleAlloc(instruction);
                break;
            case IRInstructionType::RET:
                handleRet(instruction);
                break;
        }
    }
}

void CodeGenerator::disassembleCode() const {
    csh handle;
    cs_insn *insn;
    size_t count;

    // Initialize Capstone
    if (cs_open(CS_ARCH_X86, CS_MODE_32, &handle) != CS_ERR_OK) {
        std::cerr << "Failed to initialize Capstone" << std::endl;
        return;
    }

    // Disassemble the generated code
    count = cs_disasm(handle, generatedCode.data(), generatedCode.size(), 0x1000, 0, &insn);
    if (count > 0) {
        for (size_t i = 0; i < count; i++) {
            std::cout << std::hex << std::setw(4) << std::setfill('0') << insn[i].address << " ";
            for (size_t j = 0; j < insn[i].size; j++) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)insn[i].bytes[j] << " ";
            }
            std::cout << "\t" << insn[i].mnemonic << " " << insn[i].op_str << std::endl;
        }
        cs_free(insn, count);
    } else {
        std::cerr << "Failed to disassemble code" << std::endl;
    }

    // Close Capstone
    cs_close(&handle);
}


void CodeGenerator::handleAdd(const IRInstruction& instruction) {
    uint8_t regDest = allocateRegister(instruction.dest);

    if (instruction.src2.empty()) {
        if (isInteger(instruction.src1)) {
            int32_t imm = std::stoi(instruction.src1);
            encodeImmediate(0x81, 0xC0 | regDest, imm);
        } else {
            throw std::runtime_error("Invalid immediate value for addition: " + instruction.src1);
        }
    } else {
        uint8_t regSrc1 = allocateRegister(instruction.src1);
        uint8_t regSrc2 = allocateRegister(instruction.src2);
        encodeInstruction(0x01, regSrc1, regSrc2);
        if (regDest != regSrc1) {
            encodeInstruction(0x89, regDest, regSrc1);
        }
    }
}

void CodeGenerator::handleSub(const IRInstruction& instruction) {
    uint8_t regDest = allocateRegister(instruction.dest);

    if (instruction.src2.empty()) {
        if (isInteger(instruction.src1)) {
            int32_t imm = std::stoi(instruction.src1);
            encodeImmediate(0x81, 0xE8 | regDest, imm);
        } else {
            throw std::runtime_error("Invalid immediate value for subtraction: " + instruction.src1);
        }
    } else {
        uint8_t regSrc1 = allocateRegister(instruction.src1);
        uint8_t regSrc2 = allocateRegister(instruction.src2);
        encodeInstruction(0x29, regSrc1, regSrc2);
        if (regDest != regSrc1) {
            encodeInstruction(0x89, regDest, regSrc1);
        }
    }
}

void CodeGenerator::handleMul(const IRInstruction& instruction) {
    uint8_t regDest = allocateRegister(instruction.dest);

    if (instruction.src2.empty()) {
        if (isInteger(instruction.src1)) {
            int32_t imm = std::stoi(instruction.src1);
            encodeImmediate(0x6B, regDest, imm);
        } else {
            throw std::runtime_error("Invalid immediate value for multiplication: " + instruction.src1);
        }
    } else {
        uint8_t regSrc1 = allocateRegister(instruction.src1);
        uint8_t regSrc2 = allocateRegister(instruction.src2);
        encodeInstruction(0x0F, 0xAF, regSrc1, regSrc2);
        if (regDest != regSrc1) {
            encodeInstruction(0x89, regDest, regSrc1);
        }
    }
}

void CodeGenerator::handleDiv(const IRInstruction& instruction) {
    uint8_t regDest = allocateRegister(instruction.dest);
    
    if (instruction.src2.empty()) {
        throw std::runtime_error("Division by immediate value is not supported");
    } else {
        uint8_t regSrc1 = allocateRegister(instruction.src1);
        uint8_t regSrc2 = allocateRegister(instruction.src2);

        // Move the dividend into EAX (required by IDIV)
        if (regSrc1 != 0x00) {
            encodeInstruction(0x89, 0xC0, regSrc1);
        }

        // Sign-extend EAX into EDX:EAX
        encodeInstruction(0x99); // CDQ (Convert Doubleword to Quadword)

        // Perform the division
        encodeInstruction(0xF7, 0xF8, regSrc2);

        // Move the result from EAX to the destination register if necessary
        if (regDest != 0x00) {
            encodeInstruction(0x89, regDest, 0x00);
        }
    }
}

void CodeGenerator::handleLoad(const IRInstruction& instruction) {
    uint8_t regDest = allocateRegister(instruction.dest);
    if (isInteger(instruction.src1)) {
        int32_t imm = std::stoi(instruction.src1);
        encodeImmediate(0xB8 + regDest, 0xC0 | regDest, imm);
    } else {
        uint8_t regSrc = allocateRegister(instruction.src1);
        encodeInstruction(0x8B, regDest, regSrc);
    }
}

void CodeGenerator::handleStore(const IRInstruction& instruction) {
    uint8_t regSrc = allocateRegister(instruction.src1);
    uint8_t regDest = allocateRegister(instruction.dest);
    encodeInstruction(0x89, regSrc, regDest);
}

void CodeGenerator::handleAlloc(const IRInstruction& instruction) {

    // uint8_t regDest = allocateRegister(instruction.dest);
    // if (isInteger(instruction.src1)) {
    //     int32_t imm = std::stoi(instruction.src1);
    //     encodeImmediate(0x8B, regDest, imm);
    // } else {
    //     throw std::runtime_error("Invalid immediate value for allocation: " + instruction.src1);
    // }
}

void CodeGenerator::handleRet(const IRInstruction& instruction) {
    encodeInstruction(0xC3);
}

void CodeGenerator::encodeInstruction(uint8_t opcode) {
    generatedCode.push_back(opcode);
}

void CodeGenerator::encodeInstruction(uint8_t opcode, uint8_t reg, uint8_t rm) {
    uint8_t modrm = 0xC0 | (reg << 3) | rm;
    generatedCode.push_back(opcode);
    generatedCode.push_back(modrm);
}

void CodeGenerator::encodeInstruction(uint8_t opcode1, uint8_t opcode2, uint8_t reg, uint8_t rm) {
    generatedCode.push_back(opcode1);
    generatedCode.push_back(opcode2);
    uint8_t modrm = 0xC0 | (reg << 3) | rm;
    generatedCode.push_back(modrm);
}

// void CodeGenerator::encodeImmediate(uint8_t opcode, uint8_t reg, int32_t imm) {
//     uint8_t modrm = 0xC0 | (reg << 3);
//     generatedCode.push_back(opcode);
//     generatedCode.push_back(modrm);
//     for (int i = 0; i < 4; ++i) {
//         generatedCode.push_back((imm >> (i * 8)) & 0xFF);
//     }
// }
void CodeGenerator::encodeImmediate(uint8_t opcode, uint8_t modrm, int32_t imm) {
    generatedCode.push_back(opcode);
    generatedCode.push_back(modrm);
    for (int i = 0; i < 4; ++i) {
        generatedCode.push_back((imm >> (i * 8)) & 0xFF);
    }
}

// Starting simple with 8 general purpose registers
uint8_t CodeGenerator::getRegisterCode(const std::string& reg) {
    if (reg == "EAX") return 0x00;
    if (reg == "ECX") return 0x01;
    if (reg == "EDX") return 0x02;
    if (reg == "EBX") return 0x03;
    if (reg == "ESP") return 0x04;
    if (reg == "EBP") return 0x05;
    if (reg == "ESI") return 0x06;
    if (reg == "EDI") return 0x07;
    throw std::runtime_error("Unkown register: " + reg);
}

uint8_t CodeGenerator::allocateRegister(const std::string& var) {
    if (registerMap.find(var) == registerMap.end()) {
        if (nextRegister > 7) {
            spillRegister();
        }
        registerMap[var] = nextRegister++;
    }
    return registerMap[var];
}

void CodeGenerator::spillRegister() {
    // Find the least recently used register
    std::string lruVar;
    for (const auto& entry : registerMap) {
        if (lruVar.empty() || lruVar > entry.first) {
            lruVar = entry.first;
        }
    }

    if (!lruVar.empty()) {
        uint8_t reg = registerMap[lruVar];
        encodeInstruction(0x50 + reg); // PUSH reg
        registerMap.erase(lruVar);
        nextRegister--;
    } else {
        throw std::runtime_error("Unable to spill register: no registers available");
    }
}

void CodeGenerator::reloadRegister(const std::string& var) {
    if (registerMap.find(var) == registerMap.end()) {
        throw std::runtime_error("Attempting to reload a register that was not spilled: " + var);
    }
    uint8_t reg = registerMap[var];
    encodeInstruction(0x58 + reg); // POP reg
}

bool CodeGenerator::isInteger(const std::string& s) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
    char* p;
    strtol(s.c_str(), &p, 10);
    return (*p == 0);
}