import llvmd.core;
import util;

class SymbolTable {
    static Symbol[string] symbols;
}

enum SymbolType {
    NONE,
    VARIABLE,
    OBJECT,
    CONSTANT,
    FUNCTION,
    FUNC_CALL
}

class Symbol {
    mixin ReferenceHandler;

    this() {
        type = SymbolType.NONE;
    }

    SymbolType type;
    Value value;
}

Symbol find_symbol(string s) {
    return SymbolTable.symbols.get(s, null);
}

Symbol find_or_create_symbol(string s) {
    if (s !in SymbolTable.symbols) {
        SymbolTable.symbols[s] = new Symbol;
    }
    return SymbolTable.symbols[s];
}

Symbol create_symbol(string s) {
    if (s in SymbolTable.symbols)
        return null;
    
    SymbolTable.symbols[s] = new Symbol;
    return SymbolTable.symbols[s];
}
