import llvmd.core;
import util;

class SybolTable {
    Symbol[string] symbols;
}

enum SymbolType {
    VARIABLE,
    CONSTANT,
    FUNCTION,
    FUNC_CALL
}

class Symbol {
    mixin ReferenceHandler;

    SymbolType type;
    Value value;
}

Symbol find_symbol(string s) {
    foreach_reverse (tbl; symbol_tables) {
        if (s in tbl.symbols) {
            return tbl.symbols[s];
        }
    }
    
    return null;
}

