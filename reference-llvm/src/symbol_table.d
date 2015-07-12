import std.stdio;

import llvmd.core;
import util;

class SymbolTable {
    static Symbol[string] symbols;
}

enum SymbolType {
    NONE,
    VARIABLE,
    OBJECT,
    FUNCTION,
}

class InstUsage {
    ulong pos;
    Value inst;

    this(Value inst, ulong pos) {
        this.pos = pos;
        this.inst = inst;
    }
}

class Symbol {
    mixin ReferenceHandler;

    this(string ident) {
        this.ident = ident;
        this.type = SymbolType.NONE;
    }

    string ident;

    bool is_global;
    Value parent;
    bool is_bool;

    SymbolType type;
    //Value value;

//    BasicBlock[] blocks;
    BasicBlock last_block;
    Value[BasicBlock] values;

    Value[BasicBlock] dummy;

    InstUsage[][BasicBlock] associated_const_usages;

    Type[] arg_types;
    Type return_type;

    bool implemented;
}

Symbol find_symbol(string s) {
    return SymbolTable.symbols.get(s, null);
}

Symbol find_or_create_symbol(string s) {
    if (s !in SymbolTable.symbols) {
        SymbolTable.symbols[s] = new Symbol(s);
    }
    return SymbolTable.symbols[s];
}

Symbol create_symbol(string s) {
    if (s in SymbolTable.symbols)
        return null;
    
    SymbolTable.symbols[s] = new Symbol(s);
    return SymbolTable.symbols[s];
}

Symbol[] find_symbols_in_block(BasicBlock block) {
    Symbol[] syms;
    foreach (sym; SymbolTable.symbols) {
        if (block in sym.values) {
            syms ~= sym;
        }
    }
    return syms;
}

void flush_symbols(Value parent) {
    string[] flush;
    foreach (name, sym; SymbolTable.symbols) {
        if (!sym.is_global && (sym.parent == parent || parent is null))
            flush ~= name;
    }
    foreach (name; flush) {
        SymbolTable.symbols.remove(name);
    }
}
