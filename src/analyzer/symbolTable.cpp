#include "symbolTable.h"


void SymbolTable::enterScope() {
    scopeStack.emplace_back();
    currentScope = scopeStack.back();
}

void SymbolTable::exitScope() {
    if (!scopeStack.empty()) {
        scopeStack.pop_back();
        if (!scopeStack.empty()) {
            currentScope = scopeStack.back();
        } else {
            currentScope = std::unordered_map<std::string, SymbolInfo>();
            throw std::runtime_error("No scope to exit");
        }
    }
}

void SymbolTable::insert(const std::string& name, SymbolType type, const std::string& dataType) {
    if(currentScope.find(name) != currentScope.end()) {
        throw std::runtime_error("Symbol '" + name + "' already declared in the current scope");
    }
    SymbolInfo s_info = {type, dataType};
    currentScope.insert({name, s_info});
}

bool SymbolTable::lookup(const std::string& name, SymbolInfo& info) {
    auto scope = scopeStack.rbegin();

    while (scope != scopeStack.rend()) {
        auto it = scope->find(name);
        if(it != scope->end()) {
            info = it->second;
            return true;
        }
        ++scope;
    }

    auto it = currentScope.find(name);
    if(it != currentScope.end()) {
        info = it->second;
        return true;
    }
    
    return false;
}
