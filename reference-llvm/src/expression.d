import llvmd.core;
import util, symbol_table, errors, manifest;

import std.conv;
import std.stdio;

Module current_module;
Builder current_builder;

Value current_function;
BasicBlock current_block;

Value current_value;

Type object_type, numeric_type, bool_type, string_type;
Value void_value, false_value, true_value,
      false_numeric_value, true_numeric_value;

Value yield_fn, fork_fn;

SystemCall[string] system_calls;

IfElse active_ifelse;

Manifest current_manifest;

class Expression {
    mixin ReferenceHandler;
    Value value;
    bool is_bool;
    Symbol const_is_sym;
}

class Arguments {
    mixin ReferenceHandler;
    Type[] types;
    Symbol[] symbols;
}

class Params {
    mixin ReferenceHandler;

    Value[] values;
    Symbol[] const_syms;
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
    bool string_arg, string_arg_replaces_arg;
    int[] defaults;

    this(string name, ulong args, bool string_arg, bool string_arg_replaces_arg = false) {
        this.args = args;
        this.string_arg = string_arg;
        this.string_arg_replaces_arg = string_arg_replaces_arg;
        Type[] arg_types;
        foreach (n; 0 .. args)
            arg_types ~= numeric_type;

        if (string_arg) {
            arg_types ~= string_type;
        }

        fn = current_module.add_function(name, Type.function_type(numeric_type, arg_types));
    }
}

void init(string manifest_file) {
    current_module = Module.create_with_name("main_module");
    current_builder = Builder.create();
    numeric_type = Type.int_type(32);
    bool_type = Type.int_type(1);
    object_type = Type.struct_type([], false);
    string_type = Type.pointer_type(Type.int_type(8));
    void_value = Value.create_const_int(numeric_type, 0);
    true_value = Value.create_const_int(bool_type, 1);
    false_value = Value.create_const_int(bool_type, 0);
    true_numeric_value = Value.create_const_int(numeric_type, 1);
    false_numeric_value = Value.create_const_int(numeric_type, 0);


//    write_function = current_module.add_function("___write_builtin", Type.function_type(numeric_type, [numeric_type]));
//    read_function = current_module.add_function("___read_builtin", Type.function_type(numeric_type, [numeric_type]));

    system_calls["write"] = new SystemCall("___write_builtin", 2, true, true);
    system_calls["read"] = new SystemCall("___read_builtin", 1, false);

    yield_fn = current_module.add_function("___yield_builtin", Type.function_type(numeric_type, []));
    fork_fn = current_module.add_function("___fork_builtin", Type.function_type(numeric_type, [Type.pointer_type(Type.function_type(numeric_type, []))]));

    if (manifest_file.length > 0)
        current_manifest = Manifest.load(File(manifest_file, "r"));
}

bool unimplemented_functions() {
    bool errors = false;
    foreach (name, sym; SymbolTable.symbols) {
        if (sym.type == SymbolType.FUNCTION && !sym.implemented) {
//            stderr.writeln("error: unimplemented function ", name);
            error_unimplemented_function(name);
            errors = true;
        }
    }
    return errors;
}

extern (C) {
    ulong expr_atom_ident(char *s) {
        auto sym = find_symbol(text(s));
        if (sym is null) {
            error_unknown_identifier(text(s));
//            stderr.writeln("error: ", text(s), " nonexistant (line ",yylineno,")");
//            generic_error();
//            error_occured++;
//            return ulong.max;
        }
        auto res = new Expression;

        if (sym.is_global && sym.parent != current_function) {
            res.value = current_builder.load(sym.global_value);
            res.is_bool = false;
            sym.parent = current_function;
        } else {
            res.value = sym.values[sym.last_block];
            res.is_bool = sym.is_bool;
            if (res.value.is_const())
                res.const_is_sym = sym;
        }
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

    ulong expr_atom_bool(int is_true) {
        auto res = new Expression;
        res.is_bool = true;
        if (is_true == 1) {
            res.value = true_value;
        } else {
            res.value = false_value;
        }

        return res.reference();
    }

    ulong expr_atom_syscall(char *ident, ulong qident_ref, ulong params_ref) {
        auto q = QualifiedIdentifier.lookup(qident_ref);
        auto p = Params.lookup(params_ref);
        auto res = new Expression;

        auto call = system_calls.get(text(ident), null);

        if (call is null) {
//            stderr.writeln("error: no such system intrinsic ",text(ident), " (line ",yylineno,")");
//            generic_error();
            error_unknown_intrinsic(text(ident));
        }
        /*
        if (call.args != p.values.length + 1) {
            stderr.writeln("error: incorrect number of parameters for intrinsic ",text(ident), " (line ",yylineno,")");
            generic_error();
        }
        */

        if (current_manifest is null) {
//            stderr.writeln("error: no manifest loaded (line ",yylineno,")");
//            generic_error();
            error_no_manifest();
        }

        string qname;
        foreach (s; q.idents)
            qname ~= "."~s;

        if (qname !in current_manifest.entries) {
//            stderr.writeln("error: no such object ",qname, " (line ",yylineno,")");
//            generic_error();
            error_not_in_manifest(qname);
        }

        ushort[2] qn = current_manifest.entries[qname];

        Value[] praw = [Value.create_const_int(numeric_type, (qn[0] | (qn[1] << 16)))];
        string[] prawstr;

        foreach (v; p.values) {
            if (v.is_constant_string()) {
                prawstr ~= text(v.as_string);
            } else if (v.type.same(bool_type)) {
                praw ~= current_builder.select(v, true_numeric_value, false_numeric_value);
            } else {
                praw ~= v;
            }
        }
        
        if (prawstr.length > 0 && call.string_arg_replaces_arg) {
            praw ~= Value.create_const_int(numeric_type, 0);
        }

        if (praw.length != call.args) {
//            stderr.writeln("error: incorrect number of parameters for intrinsic ",text(ident), " (line ",yylineno,")");
//            generic_error();
            error_intrinsic_parameter_count(text(ident));
        }

        if (prawstr.length > 0 && !call.string_arg) {
//            stderr.writeln("error: system intrinsic ",text(ident), " does not accept strings (line ",yylineno,")");
//            generic_error();
            error_intrinsic_unhandled_string(text(ident));
        }

        if (call.string_arg) {
            Value strv;
            if (prawstr.length == 0) {
                strv = Value.create_const_null(string_type);
            } else {
                string str;
                foreach (s; prawstr) {
                    str ~= s ~ '\0';
                }
                strv = current_builder.global_string_ptr(str);
            }
            res.value = current_builder.call(call.fn, praw ~ strv);
        } else {
            res.value = current_builder.call(call.fn, praw);
        }

        return res.reference();
    }

    ulong expr_op_lor(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (!lhs.is_bool || !rhs.is_bool) {
//            stderr.writeln("error: 'or' must be used with boolean values (line ",yylineno,")");
//            generic_error();
            error_logical_op_requires_booleans("or");
        }

        auto a = current_builder.icmp_ne(lhs.value, false_value);
        auto b = current_builder.icmp_ne(rhs.value, false_value);

        auto res = new Expression;
        res.value = current_builder.select(a, true_value, b);
        res.is_bool = true;

        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }

        return res.reference();
    }

    ulong expr_op_lxor(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (!lhs.is_bool || !rhs.is_bool) {
//            stderr.writeln("error: 'xor' must be used with boolean values (line ",yylineno,")");
//            generic_error();
            error_logical_op_requires_booleans("xor");
        }

        auto a = current_builder.icmp_ne(lhs.value, false_value);
        auto b = current_builder.icmp_ne(rhs.value, false_value);

        auto res = new Expression;
        res.value = current_builder.icmp_ne(a, b);
        res.is_bool = true;

        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        return res.reference();
    }

    ulong expr_op_land(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (!lhs.is_bool || !rhs.is_bool) {
//            stderr.writeln("error: 'and' must be used with boolean values (line ",yylineno,")");
//            generic_error();
            error_logical_op_requires_booleans("and");
        }

        auto a = current_builder.icmp_ne(lhs.value, false_value);
        auto b = current_builder.icmp_ne(rhs.value, false_value);

        auto res = new Expression;
        res.value = current_builder.select(a, b, false_value);
        res.is_bool = true;
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        return res.reference();
    }

    ulong expr_op_eq(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        /*
        if (lhs.is_bool != rhs.is_bool) {
            stderr.writeln("error: cannot compare boolean and non-boolean values (line ",yylineno,")");
            generic_error();
        }
        */

        Value lval, rval;

        if (lhs.is_bool && !rhs.is_bool) {
            lval = lhs.value;
            rval = current_builder.icmp_ne(rhs.value, false_numeric_value);
        } else if (!lhs.is_bool && rhs.is_bool) {
            rval = rhs.value;
            lval = current_builder.icmp_ne(lhs.value, false_numeric_value);
        } else {
            lval = lhs.value;
            rval = rhs.value;
        }

        auto res = new Expression;
        res.value = current_builder.icmp_eq(lval, rval);
        res.is_bool = true;
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        return res.reference();
    }

    ulong expr_op_lt(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        /*
        if (lhs.is_bool != rhs.is_bool) {
            stderr.writeln("error: cannot compare boolean and non-boolean values (line ",yylineno,")");
            generic_error();
        }
        */
        Value lval, rval;

        if (lhs.is_bool && !rhs.is_bool) {
            lval = lhs.value;
            rval = current_builder.icmp_ne(rhs.value, false_numeric_value);
        } else if (!lhs.is_bool && rhs.is_bool) {
            lval = current_builder.icmp_ne(lhs.value, false_numeric_value);
            rval = rhs.value;
        } else {
            lval = lhs.value;
            rval = rhs.value;
        }

        auto res = new Expression;
        res.value = current_builder.icmp_slt(lval, rval);
        res.is_bool = true;
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        return res.reference();
    }

    ulong expr_op_gt(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        /*
        if (lhs.is_bool != rhs.is_bool) {
            stderr.writeln("error: cannot compare boolean and non-boolean values (line ",yylineno,")");
            generic_error();
        }
        */
        Value lval, rval;

        if (lhs.is_bool && !rhs.is_bool) {
            lval = lhs.value;
            rval = current_builder.icmp_ne(rhs.value, false_numeric_value);
        } else if (!lhs.is_bool && rhs.is_bool) {
            lval = current_builder.icmp_ne(lhs.value, false_numeric_value);
            rval = rhs.value;
        } else {
            lval = lhs.value;
            rval = rhs.value;
        }

        auto res = new Expression;
        res.value = current_builder.icmp_sgt(lval, rval);
        res.is_bool = true;
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        return res.reference();
    }

    ulong expr_op_lte(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        /*
        if (lhs.is_bool != rhs.is_bool) {
            stderr.writeln("error: cannot compare boolean and non-boolean values (line ",yylineno,")");
            generic_error();
        }
        */
        Value lval, rval;

        if (lhs.is_bool && !rhs.is_bool) {
            lval = lhs.value;
            rval = current_builder.icmp_ne(rhs.value, false_numeric_value);
        } else if (!lhs.is_bool && rhs.is_bool) {
            lval = current_builder.icmp_ne(lhs.value, false_numeric_value);
            rval = rhs.value;
        } else {
            lval = lhs.value;
            rval = rhs.value;
        }

        auto res = new Expression;
        res.value = current_builder.icmp_slte(lval, rval);
        res.is_bool = true;
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        return res.reference();
    }

    ulong expr_op_gte(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        /*
        if (lhs.is_bool != rhs.is_bool) {
            stderr.writeln("error: cannot compare boolean and non-boolean values (line ",yylineno,")");
            generic_error();
        }
        */
        Value lval, rval;

        if (lhs.is_bool && !rhs.is_bool) {
            lval = lhs.value;
            rval = current_builder.icmp_ne(rhs.value, false_numeric_value);
        } else if (!lhs.is_bool && rhs.is_bool) {
            lval = current_builder.icmp_ne(lhs.value, false_numeric_value);
            rval = rhs.value;
        } else {
            lval = lhs.value;
            rval = rhs.value;
        }

        auto res = new Expression;
        res.value = current_builder.icmp_sgte(lval, rval);
        res.is_bool = true;
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        return res.reference();
    }

    ulong expr_op_add(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (lhs.value.type.same(bool_type) || rhs.value.type.same(bool_type)) {
//            stderr.writeln("error: '+' must be used with numeric values (line ",yylineno,")");
//            generic_error();
            error_arithmetic_op_requires_numerics("+");
        }

        auto res = new Expression;
        res.value = current_builder.add(lhs.value, rhs.value);
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        return res.reference();
    }

    ulong expr_op_sub(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (lhs.value.type.same(bool_type) || rhs.value.type.same(bool_type)) {
//            stderr.writeln("error: '-' must be used with numeric values (line ",yylineno,")");
//            generic_error();
            error_arithmetic_op_requires_numerics("-");
        }

        auto res = new Expression;
        res.value = current_builder.sub(lhs.value, rhs.value);
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        return res.reference();
    }

    ulong expr_op_mul(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (lhs.value.type.same(bool_type) || rhs.value.type.same(bool_type)) {
//            stderr.writeln("error: '*' must be used with numeric values (line ",yylineno,")");
//            generic_error();
            error_arithmetic_op_requires_numerics("*");
        }

        auto res = new Expression;
        res.value = current_builder.mul(lhs.value, rhs.value);
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        return res.reference();
    }

    ulong expr_op_sdiv(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (lhs.value.type.same(bool_type) || rhs.value.type.same(bool_type)) {
//            stderr.writeln("error: '/' must be used with numeric values (line ",yylineno,")");
//            generic_error();
            error_arithmetic_op_requires_numerics("/");
        }

        auto res = new Expression;
        res.value = current_builder.sdiv(lhs.value, rhs.value);
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        return res.reference();
    }

    ulong expr_op_mod(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (lhs.value.type.same(bool_type) || rhs.value.type.same(bool_type)) {
//            stderr.writeln("error: '%' must be used with numeric values (line ",yylineno,")");
//            generic_error();
            error_arithmetic_op_requires_numerics("%");
        }

        auto res = new Expression;
        res.value = current_builder.srem(lhs.value, rhs.value);
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        return res.reference();
    }

    ulong expr_op_lnot(ulong lhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);

        if (!lhs.is_bool) {
//            stderr.writeln("error: 'not' must be used with a boolean value (line ",yylineno,")");
//            generic_error();
            error_logical_op_requires_booleans("not");
        }

        auto res = new Expression;
        res.value = current_builder.icmp_eq(lhs.value, false_value);
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        return res.reference();
    }

    ulong expr_op_neg(ulong lhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);

        if (lhs.value.type.same(bool_type)) {
//            stderr.writeln("error: '-' must be used with a numeric value (line ",yylineno,")");
//            generic_error();
            error_arithmetic_op_requires_numerics("-");
        }
        auto res = new Expression;
        res.value = current_builder.neg(lhs.value);
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }

        return res.reference();
    }

    ulong expr_op_pos(ulong lhs_ref) {
        //this does nothing
        return lhs_ref;
    }

    /* Function calls */

    void create_function_call(char *ident, ulong params_ref) {
        auto p = Params.lookup(params_ref);

        foreach (ref v; p.values) {
            if (v.type.same(bool_type)) {
                v = current_builder.select(v, true_numeric_value, false_numeric_value);
//                stderr.writeln("warning: boolean parameter will be passed as numeric value (line ",yylineno,")");
                warn_boolean_parameter();
            }
        }



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
//                stderr.writeln("error: incorrect number of parameters for function ",text(ident), " (line ",yylineno,")");
//                generic_error();
                error_inconsistent_function(text(ident));
            }
        }
        current_value = current_builder.call(fn.values[null], p.values);

        foreach (i,s; p.const_syms) {
            if (s !is null) {
                s.associated_const_usages[current_block] ~= new InstUsage(current_value, i);
            }
        }
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
        sym.is_bool = rhs.is_bool;
        sym.last_block = current_block;

//        sym.value = rhs.value;

        /*
        if (rhs.is_bool) {
            current_value = current_builder.select(rhs.value, true_numeric_value, false_numeric_value);
        } else {
            current_value = rhs.value;
        }
        */
        current_value = rhs.value;
    }

    void statement_expression(ulong expr_ref) {
        auto expr = Expression.lookup(expr_ref);
        /*
        if (expr.is_bool) {
            stderr.writeln("warning: boolean expression statement (line ",yylineno,")");
            current_value = current_builder.select(expr.value, true_numeric_value, false_numeric_value);
        } else {
        */
        current_value = expr.value;
//        }
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

        Type t = null;
        foreach (ref v; phi_vals) {
            if (t is null) {
                t = v.type;
            } else if (!t.same(v.type)) {
                if (v == void_value && t.same(bool_type)) {
                    v = false_value;
                } else {
//                    stderr.writeln("error: statement may result in either boolean or numeric (line ",yylineno,")");
//                    generic_error();
                    error_indeterminate_statement_type();
                }
            }
        }

        ifelse.after_value = current_builder.make_phi(t, phi_vals, phi_blocks);

        auto syms1 = find_symbols_in_block(ifelse.during);
        if (nested_ifelse_ref == ulong.max) {
            phi_blocks = [ifelse.during, prev_block];
        } else {
            phi_blocks = [ifelse.during, nested.after];
        }

        foreach (sym; syms1) {
            phi_vals = [];
            foreach (bl; phi_blocks) {
                auto v = sym.values.get(bl, void_value);
                ulong min = 0;
                if (v == void_value) {
                    foreach (k,va; sym.values) {
                        if (k.index >= min && k.index <= ifelse.before.index && va != void_value) {
                            min = k.index;
                            v = va;
                        }
                    }
                    if (v == void_value) {
//                        stderr.writeln("warning: ",sym.ident," is conditionally defined (line ",yylineno,")");
//                        generic_error();
                        warn_conditionally_defined(sym.ident);
                    }
                }
                phi_vals ~= v;
                
            }
            t = null;
            foreach (i,v; phi_vals) {
                if (t is null) {
                    t = v.type;
                } else if (!t.same(v.type)) {
 //                   writeln("here ",i);
//                    v.dump();
//                    stderr.writeln("error: statement may result in either boolean or numeric (line ",yylineno,")");
//                    generic_error();
                    error_indeterminate_statement_type();
                }
            }
            sym.values[current_block] = current_builder.make_phi(t, phi_vals, phi_blocks);
            sym.last_block = current_block;
        }

        auto syms2 = find_symbols_in_block(ifelse.otherwise);
        phi_blocks[1] = prev_block;
        foreach (sym; syms2) {
            bool already = false;
            foreach (symprev; syms1) {
                if (symprev == sym) {
                    already = true;
                    break;
                }
            }
            if (already)
                continue;
            phi_vals = [];
            foreach (bl; phi_blocks) {
                auto v = sym.values.get(bl, void_value);
                if (v == void_value) {
                    ulong min = 0;
                    foreach (k,va; sym.values) {
                        if (k.index >= min && k.index <= ifelse.before.index && va != void_value) {
                            min = k.index;
                            v = va;
                        }
                    }
                    if (v == void_value) {
//                    stderr.writeln("warning: ",sym.ident," is conditionally defined (line ",yylineno,")");
                        warn_conditionally_defined(sym.ident);
                    }
                }
                phi_vals ~= v;
            }
            t = null;
            foreach (v; phi_vals) {
                if (t is null) {
                    t = v.type;
                } else if (!t.same(v.type)) {
//                    stderr.writeln("error: statement may result in either boolean or numeric (line ",yylineno,")");
//                    generic_error();
                    error_indeterminate_statement_type();
                }
            }
            sym.values[current_block] = current_builder.make_phi(t, phi_vals, phi_blocks);
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

        /*
        foreach (sym; SymbolTable.symbols) {
            if (sym.type != SymbolType.VARIABLE)
                continue;
            sym.values[loop.test] = Value.create_const_int(numeric_type, 0);
            sym.values[loop.during] = sym.values[loop.test];
            sym.dummy[loop.test] = sym.values[loop.test];
        }
        */
        
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

        auto syms1 = find_symbols_in_block(loop.test);
        auto syms2 = find_symbols_in_block(loop.during);

        auto syms = syms1.dup;

        foreach (sym2; syms2) {
            bool dup = false;
            foreach (sym; syms) {
                if (sym2 == sym) {
                    dup = true;
                    break;
                }
            }
            if (!dup) {
                syms ~= sym2;
            }
        }

        Value[] phi_values;
        BasicBlock[] phi_blocks = [loop.before, current_block];

        current_builder.position_before(loop.test.first_instruction());

        Value[Value] replacements;
        
        foreach (sym; syms) {
            phi_values = [];
            foreach (bl; phi_blocks) {
//                phi_values ~= sym.values.get(bl, void_value);
                auto v = sym.values.get(bl, void_value);
                ulong min = 0;
                if (v == void_value) {
                    foreach (k,va; sym.values) {
                        if (k.index >= min && k.index <= loop.before.index && va != void_value) {
                            min = k.index;
                            v = va;
                        }
                    }
                    if (v == void_value) {
//                        stderr.writeln("warning: ",sym.ident," is conditionally defined (line ",yylineno,")");
//                        generic_error();
                        warn_conditionally_defined(sym.ident);
                    }
                }
                phi_values ~= v;
            }
            Type t = null;
            foreach (v; phi_values) {
                if (t is null) {
                    t = v.type;
                } else if (!t.same(v.type)) {
//                    stderr.writeln("error: statement may result in either boolean or numeric (line ",yylineno,")");
//                    generic_error();
                    warn_unexpected_error();
                    error_indeterminate_statement_type();
                }
            }
            Value newval = current_builder.make_phi(t, phi_values, phi_blocks);
            sym.values[loop.after] = newval;
            ulong min = 0;
            BasicBlock prev_block = null;
            foreach (bl, va; sym.values) {
                if (bl.index >= min && bl.index <= loop.before.index) {
                    min = bl.index;
                    prev_block = bl;
                }
            }
            replacements[sym.values[prev_block]] = newval;
            
            sym.last_block = loop.after;
            sym.is_bool = t.same(bool_type);

            /*
            if (sym.dummy[loop.test] !is null) {
                Value.replace_all(sym.dummy[loop.test], sym.values[loop.after]);
            }
            */
        }

        Value inst = loop.test.first_instruction();

        while (inst !is null) {

            if (!inst.is_phi()) {
                foreach (i; 0 .. inst.get_num_operands()) {
                    Value operand = inst.get_operand(i);
                    if (operand.is_const()) {
                        bool stop = false;
                        foreach (sym; syms1) {
                            foreach (usage; sym.associated_const_usages.get(loop.test, [])) {
                                if (usage.inst.same(inst) && usage.pos == i) {
                                    inst.set_operand(i, sym.values[loop.after]);
                                    stop = true;
                                    break;
                                }
                            }
                            if (stop)
                                break;
                        }
                    } else {
                        foreach (k,v; replacements) {
                            if (operand.same(k)) {
                                inst.set_operand(i, v);
                                break;
                            }
                        }
                    }
                }
            }

            inst = inst.next_instruction();
        }

        inst = loop.during.first_instruction();

        while (inst !is null) {

            if (!inst.is_phi()) {
                foreach (i; 0 .. inst.get_num_operands()) {
                    Value operand = inst.get_operand(i);
                    if (operand.is_const()) {
                        foreach (sym; syms2) {
                            foreach (usage; sym.associated_const_usages.get(loop.during, [])) {
                                if (usage.inst.same(inst) && usage.pos == i) {
                                    inst.set_operand(i, sym.values[loop.after]);
                                }
                            }
                        }
                    } else {
                        foreach (k,v; replacements) {
                            if (operand.same(k)) {
                                inst.set_operand(i, v);
                            }
                        }
                    }
                }
            }

            inst = inst.next_instruction();
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
//                stderr.writeln("error: cannot fork function with arguments ", text(ident), " (line ",yylineno,")");
//                generic_error();
                error_fork_function_with_arguments(text(ident));
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
            fn.arg_types = args.types;
        } else {
            if (fn.type != SymbolType.FUNCTION) {
                error_function_shadows_different_type(text(ident));
            }
            if (fn.arg_types.length != args.types.length) {
//                stderr.writeln("error: inconsistent use of function ",text(ident), " (line ",yylineno,")");
//                generic_error();
//                error_insonsistent_function(text(ident));
                error_function_mismatch_with_prior_use(text(ident));
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
        if (current_value.type.same(bool_type)) {
//            stderr.writeln("warning: boolean function will evaluate to a numeric value (line ",yylineno,")");
            warn_boolean_return();
            current_value = current_builder.select(current_value, true_numeric_value, false_numeric_value);
        }

        foreach (sym; SymbolTable.symbols) {
            if (sym.is_global && sym.parent == current_function) {
                current_builder.store(sym.values[current_block], sym.global_value);
                sym.parent = null;
            }
        }

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
//            stderr.writeln("error: ", text(ident), " shadows (line ",yylineno,")");
//            generic_error();
            error_shadowing(text(ident));
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
//            stderr.writeln("error: ", text(ident), " shadows (line ",yylineno,")");
//            generic_error();
            error_shadowing(text(ident));
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
        auto expr = Expression.lookup(expr_ref);
        p.values ~= expr.value;
        if (expr.const_is_sym !is null) {
            p.const_syms ~= expr.const_is_sym;
        } else {
            p.const_syms ~= null;
        }
        return p.reference();
    }

    ulong params_create_string(char *s) {
        auto p = new Params;
        auto str = text(s);
        str = str[1..$-1];
        p.values ~= Value.create_string(str);
        p.const_syms ~= null;
        return p.reference();
    }

    ulong params_add(ulong param_ref, ulong expr_ref) {
        auto p = Params.lookup(param_ref);
        auto expr = Expression.lookup(expr_ref);
        p.values ~= expr.value;
        if (expr.const_is_sym !is null) {
            p.const_syms ~= expr.const_is_sym;
        } else {
            p.const_syms ~= null;
        }
        return p.reference();
    }

    ulong params_add_string(ulong param_ref, char *s) {
        auto p = Params.lookup(param_ref);
        auto str = text(s);
        str = str[1..$-1];
        p.values ~= Value.create_string(str);
        p.const_syms ~= null;
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

    /* Global */

    void global_create(char *ident, int value) {
        auto sym = create_symbol(text(ident));

        sym.is_global = true;
        sym.type = SymbolType.VARIABLE;

        sym.global_value = Value.create_global_variable(current_module, numeric_type, text(ident));
        sym.global_value.set_initializer(Value.create_const_int(numeric_type, value));
    }
}



