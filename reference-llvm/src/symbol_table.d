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
    CHANNEL,
    CONSTANT
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
    Value global_value;
    Value parent;
    bool is_bool;
    bool is_handler;

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

Symbol[] find_symbols_in_blocks(BasicBlock start, BasicBlock end) {
    BasicBlock[] selected;
    auto bbs = BasicBlock.all();
    foreach (bb; bbs) {
        if (bb.index >= start.index && bb.index < end.index) {
            selected ~= bb;
        }
    }
    Symbol[] all_syms, out_syms;
    foreach (bb; selected) {
        all_syms ~= find_symbols_in_block(bb);
    }
    foreach (s1; all_syms) {
        bool add = true;
        foreach (s2; out_syms) {
            if (s1 == s2) {
                add = false;
                break;
            }
        }
        if (add) {
            out_syms ~= s1;
        }
    }
    return out_syms;
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
