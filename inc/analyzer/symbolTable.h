#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>

enum class SymbolType {
    VARIABLE,
    FUNCTION,
    PARAMETER
};

struct SymbolInfo {
    SymbolType type;
    std::string dataType;
};

class SymbolTable {
public:
    SymbolTable() {
        enterScope();
    }
    ~SymbolTable() = default;

    void enterScope();
    void exitScope();

    void insert(const std::string& name, SymbolType type, const std::string& dataType);
    bool lookup(const std::string& name, SymbolInfo& info);

private:
    std::unordered_map<std::string, SymbolInfo> currentScope;
    std::vector<std::unordered_map<std::string, SymbolInfo>> scopeStack;
};

#endif