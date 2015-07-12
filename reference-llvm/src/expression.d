import llvmd.core;
import util, symbol_table, errors;

import std.conv;
import std.stdio;

Module current_module;
Builder current_builder;

Value current_function;
BasicBlock current_block;

Value current_value;

Type object_type, numeric_type;
Value void_value;

Value yield_fn, fork_fn;

SystemCall[string] system_calls;

IfElse active_ifelse;

class Expression {
    mixin ReferenceHandler;
    Value value;
}

class Arguments {
    mixin ReferenceHandler;
    Type[] types;
    Symbol[] symbols;
}

class Params {
    mixin ReferenceHandler;

    Value[] values;
}

class QualifiedIdentifier {
    mixin ReferenceHandler;

    string[] idents;
}

class IfElse {
    mixin ReferenceHandler;

    BasicBlock before, during, otherwise, after;

    Value during_value, otherwise_value, after_value;

    IfElse parent;
}

class While {
    mixin ReferenceHandler;
    BasicBlock before, test, during, after;
}

class SystemCall {
    ulong args;
    Value fn;

    this(string name, ulong args) {
        this.args = args;
        Type[] arg_types;
        foreach (n; 0 .. args)
            arg_types ~= numeric_type;

        fn = current_module.add_function(name, Type.function_type(numeric_type, arg_types));
    }
}

void init() {
    current_module = Module.create_with_name("main_module");
    current_builder = Builder.create();
    numeric_type = Type.int_type(32);
    object_type = Type.struct_type([], false);
    void_value = Value.create_const_int(numeric_type, 0);

//    write_function = current_module.add_function("___write_builtin", Type.function_type(numeric_type, [numeric_type]));
//    read_function = current_module.add_function("___read_builtin", Type.function_type(numeric_type, [numeric_type]));

    system_calls["write"] = new SystemCall("___write_builtin", 2);
    system_calls["read"] = new SystemCall("___read_builtin", 1);

    yield_fn = current_module.add_function("___yield_builtin", Type.function_type(numeric_type, []));
    fork_fn = current_module.add_function("___fork_builtin", Type.function_type(numeric_type, [Type.pointer_type(Type.function_type(numeric_type, []))]));
}

bool unimplemented_functions() {
    bool errors = false;
    foreach (name, sym; SymbolTable.symbols) {
        if (sym.type == SymbolType.FUNCTION && !sym.implemented) {
            stderr.writeln("error: unimplemented function ", name);
            errors = true;
        }
    }
    return errors;
}

extern (C) {
    ulong expr_atom_ident(char *s) {
        auto sym = find_symbol(text(s));
        if (sym is null) {
            stderr.writeln("error: ", text(s), " nonexistant");
            generic_error();
//            error_occured++;
//            return ulong.max;
        }
        auto res = new Expression;
        writeln(sym.last_block);
        res.value = sym.values[sym.last_block];
        return res.reference();
    }

    ulong expr_atom_numeric(ulong n) {
        auto res = new Expression;
        res.value = Value.create_const_int(numeric_type, n);

        return res.reference();
    }

    ulong expr_atom_function_call() {
        auto res = new Expression;
        res.value = current_value;
        return res.reference();
    }

    ulong expr_atom_syscall(char *ident, ulong qident_ref, ulong params_ref) {
        auto q = QualifiedIdentifier.lookup(qident_ref);
        auto p = Params.lookup(params_ref);
        auto res = new Expression;

        auto call = system_calls.get(text(ident), null);

        if (call is null) {
            stderr.writeln("error: no such system intrinsic ",text(ident));
            generic_error();
        }
        if (call.args != p.values.length + 1) {
            stderr.writeln("error: incorrect number of parameters for intrinsic ",text(ident));
            generic_error();
        }

        Value[] praw = [Value.create_const_int(numeric_type, 0)];

        foreach (v; p.values)
            praw ~= v;

        res.value = current_builder.call(call.fn, praw);

        return res.reference();
    }

    ulong expr_op_lor(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto a = current_builder.icmp_ne(lhs.value, Value.create_const_int(numeric_type, 0));
        auto b = current_builder.icmp_ne(rhs.value, Value.create_const_int(numeric_type, 0));

        auto res = new Expression;
        res.value = current_builder.select(a, Value.create_const_int(numeric_type, 1), b);

        return res.reference();
    }

    ulong expr_op_lxor(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto a = current_builder.icmp_ne(lhs.value, Value.create_const_int(numeric_type, 0));
        auto b = current_builder.icmp_ne(rhs.value, Value.create_const_int(numeric_type, 0));

        auto res = new Expression;
        res.value = current_builder.icmp_ne(a, b);

        return res.reference();
    }

    ulong expr_op_land(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto a = current_builder.icmp_ne(lhs.value, Value.create_const_int(numeric_type, 0));
        auto b = current_builder.icmp_ne(lhs.value, Value.create_const_int(numeric_type, 0));

        auto res = new Expression;
        res.value = current_builder.select(a, b, Value.create_const_int(numeric_type, 0));

        return res.reference();
    }

    ulong expr_op_eq(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto res = new Expression;
        res.value = current_builder.icmp_eq(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_lt(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto res = new Expression;
        res.value = current_builder.icmp_slt(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_gt(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto res = new Expression;
        res.value = current_builder.icmp_sgt(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_lte(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto res = new Expression;
        res.value = current_builder.icmp_slte(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_gte(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto res = new Expression;
        res.value = current_builder.icmp_sgte(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_add(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto res = new Expression;
        res.value = current_builder.add(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_sub(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto res = new Expression;
        res.value = current_builder.sub(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_mul(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto res = new Expression;
        res.value = current_builder.mul(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_sdiv(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto res = new Expression;
        res.value = current_builder.sdiv(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_mod(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto res = new Expression;
        res.value = current_builder.srem(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_lnot(ulong lhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);

        auto res = new Expression;
        res.value = current_builder.icmp_eq(lhs.value, Value.create_const_int(numeric_type, 0));

        return res.reference();
    }

    ulong expr_op_neg(ulong lhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);

        auto res = new Expression;
        res.value = current_builder.neg(lhs.value);

        return res.reference();
    }

    ulong expr_op_pos(ulong lhs_ref) {
        //this does nothing
        return lhs_ref;
    }

    /* Function calls */

    void create_function_call(char *ident, ulong params_ref) {
        auto p = Params.lookup(params_ref);

        auto fn = find_symbol(text(ident));
        if (fn is null) {
            fn = create_symbol(text(ident));
            fn.is_global = true;
            fn.type = SymbolType.FUNCTION;
            Type[] types;
            foreach (n; 0 .. p.values.length)
                types ~= numeric_type;
            fn.values[null] = current_module.add_function(text(ident), Type.function_type(numeric_type, types));
            fn.arg_types = types;
        } else {
            if (fn.arg_types.length != p.values.length) {
                stderr.writeln("error: incorrect number of parameters for function ",text(ident));
                generic_error();
            }
        }
        current_value = current_builder.call(fn.values[null], p.values);
    }

    /* Statements */

    void statement_assign(char *lhs, ulong rhs_ref) {
        auto sym = find_or_create_symbol(text(lhs));
        if (sym.parent is null) {
            sym.parent = current_function;
        }
        auto rhs = Expression.lookup(rhs_ref);

        sym.type = SymbolType.VARIABLE;

        sym.values[current_block] = rhs.value;
        sym.last_block = current_block;

//        sym.value = rhs.value;

        current_value = rhs.value;
    }

    void statement_expression(ulong expr_ref) {
        auto expr = Expression.lookup(expr_ref);
        current_value = expr.value;
    }

    /*
    void statement_return_void() {
        current_builder.ret_void();
    }

    void statement_return_expr(ulong rhs_ref) {
        auto rhs = Expression.lookup(rhs_ref);
        current_builder.ret(rhs.value);
    }
    */

    ulong statement_if_begin(ulong cond_ref) {
        auto cond = Expression.lookup(cond_ref);

        auto ifelse = new IfElse;

        if (active_ifelse is null) {
            ifelse.before = current_block;
        } else {
            ifelse.before = active_ifelse.before;
            ifelse.parent = active_ifelse;
            active_ifelse = null;
        }

        ifelse.during = current_function.append_basic_block(null);
        ifelse.otherwise = current_function.append_basic_block(null);
        ifelse.after = current_function.append_basic_block(null);

        current_value = void_value;
        ifelse.during_value = void_value;
        ifelse.otherwise_value = void_value;

        current_builder.cond_br(cond.value, ifelse.during, ifelse.otherwise);
        current_builder.position_at_end(ifelse.during);
        current_block = ifelse.during;

        return ifelse.reference();
    }

    void statement_if_break(ulong ifelse_ref) {
        auto ifelse = IfElse.lookup(ifelse_ref);
        current_builder.br(ifelse.after);
        current_builder.position_at_end(ifelse.otherwise);
        ifelse.during = current_block;
        current_block = ifelse.otherwise;
        ifelse.during_value = current_value;
        current_value = void_value;

        auto syms = find_symbols_in_block(ifelse.during);
        foreach (sym; syms) {
            sym.last_block = ifelse.before;
        }

        active_ifelse = ifelse;
    }

    ulong statement_if_end(ulong ifelse_ref, ulong nested_ifelse_ref) {
        auto ifelse = IfElse.lookup(ifelse_ref);
        IfElse nested;
        Value[] phi_vals;
        BasicBlock[] phi_blocks;
        
        ifelse.otherwise_value = current_value;

        if (nested_ifelse_ref == ulong.max) {
            nested = null;
            phi_vals = [ifelse.during_value, ifelse.otherwise_value];
            phi_blocks = [ifelse.during, current_block];
        } else {
            nested = IfElse.lookup(nested_ifelse_ref);
            phi_vals = [ifelse.during_value, nested.after_value];
            phi_blocks = [ifelse.during, nested.after];
        }

        current_builder.br(ifelse.after);
        current_builder.position_at_end(ifelse.after);
        auto prev_block = current_block;
        current_block = ifelse.after;
        ifelse.after_value = current_builder.make_phi(numeric_type, phi_vals, phi_blocks);

        auto syms1 = find_symbols_in_block(ifelse.during);
        if (nested_ifelse_ref == ulong.max) {
            phi_blocks = [ifelse.during, prev_block];
        } else {
            phi_blocks = [ifelse.during, nested.after];
        }

        foreach (sym; syms1) {
            phi_vals = [];
            foreach (bl; phi_blocks) {
                phi_vals ~= sym.values.get(bl, void_value);
            }
            sym.values[current_block] = current_builder.make_phi(numeric_type, phi_vals, phi_blocks);
            sym.last_block = current_block;
        }

        auto syms2 = find_symbols_in_block(ifelse.otherwise);
        phi_blocks[1] = prev_block;
        foreach (sym; syms2) {
            phi_vals = [];
            foreach (bl; phi_blocks) {
                phi_vals ~= sym.values.get(bl, void_value);
            }
            sym.values[current_block] = current_builder.make_phi(numeric_type, phi_vals, phi_blocks);
            sym.last_block = current_block;
        }

        current_value = ifelse.after_value;

        if (active_ifelse == ifelse) {
            active_ifelse = ifelse.parent;
        }

        return ifelse_ref;
    }

    ulong statement_else_terminal() {
        return ulong.max;
    }

    ulong statement_while_begin() {
        auto loop = new While;

        current_value = void_value;
        loop.before = current_block;
        loop.test = current_function.append_basic_block(null);
        loop.during = current_function.append_basic_block(null);
        loop.after = current_function.append_basic_block(null);

        current_builder.br(loop.test);
        current_builder.position_at_end(loop.test);

        current_block = loop.test;
        
        return loop.reference();
    }

    ulong statement_while_begin_do(ulong loop_ref) {
        auto loop = While.lookup(loop_ref);
        
        current_builder.cond_br(current_value, loop.during, loop.after);
        current_builder.position_at_end(loop.during);

        current_block = loop.during;

        current_value = void_value;

        return loop.reference();
    }

    void statement_while_end(ulong loop_ref) {
        auto loop = While.lookup(loop_ref);

        current_builder.br(loop.test);

        auto syms = find_symbols_in_block(loop.during) ~ find_symbols_in_block(loop.test);

        Value[] phi_values;
        BasicBlock[] phi_blocks = [loop.before, loop.during];

        current_builder.position_before(loop.test.first_instruction());

        
        foreach (sym; syms) {
            phi_values = [];
            foreach (bl; phi_blocks) {
                phi_values ~= sym.values.get(bl, void_value);
            }
            sym.values[loop.after] = current_builder.make_phi(numeric_type, phi_values, phi_blocks);
            sym.last_block = loop.after;
        }
        
        current_builder.position_at_end(loop.after);
        current_value = void_value;

        current_block = loop.after;
    }

    void statement_yield() {
        current_value = void_value;
        current_builder.call(yield_fn, []);
    }

    void statement_fork(char *ident) {
        current_value = void_value;
        auto fn = find_symbol(text(ident));
        if (fn is null) {
            fn = create_symbol(text(ident));
            fn.is_global = true;
            fn.type = SymbolType.FUNCTION;
            fn.values[null] = current_module.add_function(text(ident), Type.function_type(numeric_type, []));
        } else {
            if (fn.arg_types.length != 0) {
                stderr.writeln("error: cannot fork function with arguments ", text(ident));
                generic_error();
            }
        }
        current_builder.call(fork_fn, [fn.values[null]]);
        current_value = void_value;
    }

    /* Functions */

    void function_begin(char *ident, ulong args_ref, int returns_object) {
        auto args = Arguments.lookup(args_ref);
        auto fn = find_symbol(text(ident));
        if (fn is null) {
            current_function = current_module.add_function(text(ident), Type.function_type(numeric_type, args.types));
            fn = create_symbol(text(ident));
            fn.is_global = true;
            fn.type = SymbolType.FUNCTION;
            fn.values[null] = current_function;
        } else {
            if (fn.arg_types.length != args.types.length) {
                stderr.writeln("error: inconsistant use of function ",text(ident));
                generic_error();
            }
            current_function = fn.values[null];
        }
        fn.implemented = true;
        current_block = current_function.append_basic_block("entry");
        current_builder.position_at_end(current_block);

        current_value = void_value;

        foreach (i, sym; args.symbols) {
            sym.values[current_block] = current_function.get_param(cast(uint)i);
            sym.parent = current_function;
            sym.last_block = current_block;
        }
    }

    void function_end() {
        current_builder.ret(current_value);
        flush_symbols(current_function);
    }

    /* Arguments */
    ulong args_empty() {
        auto a = new Arguments;
        return a.reference();
    }

    ulong args_create(char *ident, int is_object) {
        auto a = new Arguments;
        auto sym = create_symbol(text(ident));
        if (sym is null) {
            stderr.writeln("error: ", text(ident), " shadows");
            generic_error();
        }
        if (is_object) {
            a.types ~= object_type;
            sym.type = SymbolType.OBJECT;
        } else {
            a.types ~= numeric_type;
            sym.type = SymbolType.VARIABLE;
        }
        a.symbols ~= sym;
        return a.reference();
    }

    ulong args_add(ulong args_ref, char *ident, int is_object) {
        auto a = Arguments.lookup(args_ref);
        auto sym = create_symbol(text(ident));
        sym.parent = current_function;
        if (sym is null) {
            stderr.writeln("error: ", text(ident), " shadows");
            generic_error();
        }
        if (is_object) {
            a.types ~= object_type;
            sym.type = SymbolType.OBJECT;
        } else {
            a.types ~= numeric_type;
            sym.type = SymbolType.VARIABLE;
        }
        a.symbols ~= sym;
        return a.reference();
    }

    /* Parameters */

    ulong params_empty() {
        auto p = new Params;
        return p.reference();
    }

    ulong params_create(ulong expr_ref) {
        auto p = new Params;
        p.values ~= Expression.lookup(expr_ref).value;
        return p.reference();
    }

    ulong params_add(ulong param_ref, ulong expr_ref) {
        auto p = Params.lookup(param_ref);
        p.values ~= Expression.lookup(expr_ref).value;

        return p.reference();
    }

    /* Qualified Identifier */

    ulong qident_create(char *ident) {
        auto q = new QualifiedIdentifier;
        q.idents ~= text(ident);
        return q.reference();
    }

    ulong qident_add(ulong qident_ref, char *ident) {
        auto q = QualifiedIdentifier.lookup(qident_ref);
        q.idents ~= text(ident);
        return q.reference();
    }
}



