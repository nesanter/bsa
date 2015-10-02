import llvmd.core;
import util, symbol_table, errors, manifest;

import std.conv;
import std.stdio;

Module current_module;
Builder current_builder;

Value entry_function;
Value current_function;
BasicBlock current_block;

Value current_value;
Value current_eh;
bool[3] current_function_has_handlers;

Type object_type, numeric_type, bool_type, string_type, eh_type, eh_ptr_type, ex_info_type;
Value void_value, false_value, true_value,
      false_numeric_value, true_numeric_value,
      eh_default_flags, null_eh;

Value yield_fn, fork_fn, fail_fn, trace_fn;

SystemCall[string] system_calls;

IfElse active_ifelse;

Manifest current_manifest;

bool include_trace_names = false;

enum HandlerEntryField {
    PARENT = 0,
    NAME = 1,
    FLAGS = 2,
    ALWAYS = 3,
    SUCCESS = 4,
    FAILURE = 5
}

enum HandlerNumber {
    ALWAYS = 0,
    SUCCESS = 1,
    FAILURE = 2
}

enum FunctionAttribute {
    NONE = 0UL,
    PARENTSCOPE = 1UL,
    HANDLER = 2UL,
    ENTRY = 4UL,
    NONEXISTANT = 0xFFFFFFFFFFFFFFFFUL
}
enum function_attributes = [
    "none" : FunctionAttribute.NONE,
    "parentscope" : FunctionAttribute.PARENTSCOPE,
    "handler" : FunctionAttribute.HANDLER,
    "entry" : FunctionAttribute.ENTRY
];

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
    bool is_read, is_write;
    ulong args;
    Value fn;
    bool string_arg, string_arg_replaces_arg;
    int[] defaults;

    this(string name, ulong args, bool string_arg, bool string_arg_replaces_arg = false) {
        this.args = args;
        this.string_arg = string_arg;
        this.string_arg_replaces_arg = string_arg_replaces_arg;
        Type[] arg_types = [eh_ptr_type];
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
    ex_info_type = numeric_type;
    eh_type = Type.named_struct_type("eh_t");
    eh_type.set_body([
        Type.pointer_type(eh_type),
        Type.pointer_type(Type.int_type(8)),
        Type.int_type(32),
        Type.pointer_type(Type.function_type(Type.void_type(), [])), //always
        Type.pointer_type(Type.function_type(Type.void_type(), [])), //success
        Type.pointer_type(Type.function_type(Type.void_type(), [ex_info_type])) //failure
    ], false);
    eh_ptr_type = Type.pointer_type(eh_type);
    string_type = Type.pointer_type(Type.int_type(8));
    void_value = Value.create_const_int(numeric_type, 0);
    true_value = Value.create_const_int(bool_type, 1);
    false_value = Value.create_const_int(bool_type, 0);
    true_numeric_value = Value.create_const_int(numeric_type, 1);
    false_numeric_value = Value.create_const_int(numeric_type, 0);
    eh_default_flags = Value.create_const_int(numeric_type, 0);
    null_eh = Value.create_const_null(eh_ptr_type);


//    write_function = current_module.add_function("___write_builtin", Type.function_type(numeric_type, [numeric_type]));
//    read_function = current_module.add_function("___read_builtin", Type.function_type(numeric_type, [numeric_type]));

    system_calls["write"] = new SystemCall("___write_builtin", 2, true, true);
    system_calls["read"] = new SystemCall("___read_builtin", 1, false);
    system_calls["block"] = new SystemCall("___block_builtin", 1, false);
    system_calls["store"] = new SystemCall("___write_addr_builtin", 3, false);
    system_calls["load"] = new SystemCall("___read_addr_builtin", 2, false);

    yield_fn = current_module.add_function("___yield_builtin", Type.function_type(Type.void_type(), []));
    fork_fn = current_module.add_function("___fork_builtin", Type.function_type(Type.void_type(), [eh_ptr_type, Type.pointer_type(Type.function_type(numeric_type, [eh_ptr_type]))]));
    fail_fn = current_module.add_function("___fail_builtin", Type.function_type(Type.void_type(), [eh_ptr_type]));
    trace_fn = current_module.add_function("___trace_builtin", Type.function_type(Type.void_type(), [eh_ptr_type]));

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
        if (sym.type != SymbolType.VARIABLE) {
            error_symbol_of_different_type(text(s));
        }
        auto res = new Expression;

        if (sym.is_global && sym.parent != current_function) {
            res.value = current_builder.load(sym.global_value);
            if (sym.is_bool) {
                res.value = current_builder.icmp_ne(res.value, false_numeric_value);
            }
            res.is_bool = sym.is_bool;
//            sym.parent = current_function;
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

        auto ent = current_manifest.entries[qname];

        Value[] praw = [Value.create_const_int(numeric_type, ent.index)];
        string[] prawstr;
        bool has_bool_args;

        foreach (v; p.values) {
            if (v.is_constant_string()) {
                prawstr ~= text(v.as_string);
            } else if (v.type.same(bool_type)) {
                has_bool_args = true;
                praw ~= current_builder.select(v, true_numeric_value, false_numeric_value);
            } else {
                praw ~= v;
            }
        }

        if (!ent.syscall_allowed(text(ident))) {
            error_intrinsic_not_allowed_for_target(text(ident), qname);
        }
        if (!ent.syscall_arguments_allowed(prawstr.length > 0, praw.length > 1, has_bool_args)) {
            error_intrinsic_args_not_allowed_for_target(text(ident), qname);
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
            if (current_eh is null) {
                res.value = current_builder.call(call.fn, [null_eh] ~ praw ~ strv);
            } else {
                res.value = current_builder.call(call.fn, [current_eh] ~ praw ~ strv);
            }
        } else {
            if (current_eh is null) {
                res.value = current_builder.call(call.fn, [null_eh] ~ praw);
            } else {
                res.value = current_builder.call(call.fn, [current_eh] ~ praw);
            }
        }

        if (ent.syscall_boolean(text(ident))) {
            res.value = current_builder.icmp_ne(res.value, false_numeric_value);
            res.is_bool = true;
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

        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        */

        return res.reference();
    }

    ulong expr_op_bor(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (lhs.is_bool || rhs.is_bool) {
            error_arithmetic_op_requires_numerics("|");
        }
        auto res = new Expression;
        res.value = current_builder.bor(lhs.value, rhs.value);
        
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

        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        */
        return res.reference();
    }

    ulong expr_op_bxor(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (lhs.is_bool || rhs.is_bool) {
            error_arithmetic_op_requires_numerics("^");
        }
        auto res = new Expression;
        res.value = current_builder.bxor(lhs.value, rhs.value);
        
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
        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        */
        return res.reference();
    }

    ulong expr_op_band(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (lhs.is_bool || rhs.is_bool) {
            error_arithmetic_op_requires_numerics("&");
        }
        auto res = new Expression;
        res.value = current_builder.band(lhs.value, rhs.value);
        
        return res.reference();
    }

    ulong expr_op_is(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (!rhs.is_bool) {
            error_is_op_requires_rh_boolean("is");
        }

        Value tmp;
        if (lhs.is_bool) {
            tmp = current_builder.icmp_ne(lhs.value, false_value);
        } else {
            tmp = current_builder.icmp_ne(lhs.value, false_numeric_value);
        }

        auto res = new Expression;
        res.value = current_builder.icmp_eq(tmp, rhs.value);
        res.is_bool = true;

        return res.reference();
    }

    ulong expr_op_nis(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (!rhs.is_bool) {
            error_is_op_requires_rh_boolean("!is");
        }

        Value tmp;
        if (lhs.is_bool) {
            tmp = current_builder.icmp_ne(lhs.value, false_value);
        } else {
            tmp = current_builder.icmp_ne(lhs.value, false_numeric_value);
        }

        auto res = new Expression;
        res.value = current_builder.icmp_ne(tmp, rhs.value);
        res.is_bool = true;

        return res.reference();
    }
    ulong expr_op_shl(ulong lhs_ref, ulong constant) {
        auto lhs = Expression.lookup(lhs_ref);
//        auto rhs = Expression.lookup(rhs_ref);
        auto rhs = Value.create_const_int(numeric_type, constant);

        // shifts > 31 are undefined per LLVM standards
        // fix that here
        if (constant > 31) {
            auto res = new Expression;
            res.value = Value.create_const_int(numeric_type, 0);
            return res.reference();
        }

        if (lhs.is_bool) {
            error_arithmetic_op_requires_numerics("<<");
        }
        auto res = new Expression;
        res.value = current_builder.shl(lhs.value, rhs);
        
        return res.reference();
    }
    
    ulong expr_op_shrl(ulong lhs_ref, ulong constant) {
        auto lhs = Expression.lookup(lhs_ref);
//        auto rhs = Expression.lookup(rhs_ref);
        auto rhs = Value.create_const_int(numeric_type, constant);

        // shifts > 31 are undefined per LLVM standards
        // fix that here
        if (constant > 31) {
            auto res = new Expression;
            res.value = Value.create_const_int(numeric_type, 0);
            return res.reference();
        }

        if (lhs.is_bool) {
            error_arithmetic_op_requires_numerics(">>");
        }
        auto res = new Expression;
        res.value = current_builder.shrl(lhs.value, rhs);
        
        return res.reference();
    }
    
    ulong expr_op_shra(ulong lhs_ref, ulong constant) {
        auto lhs = Expression.lookup(lhs_ref);
//        auto rhs = Expression.lookup(rhs_ref);
        auto rhs = Value.create_const_int(numeric_type, constant);

        // shifts > 31 are undefined per LLVM standards
        // fix that here
        if (constant > 31) {
            auto res = new Expression;
            res.value = Value.create_const_int(numeric_type, 0xFFFFFFFFU);
            return res.reference();
        }

        if (lhs.is_bool) {
            error_arithmetic_op_requires_numerics(">>>");
        }
        auto res = new Expression;
        res.value = current_builder.shra(lhs.value, rhs);
        
        return res.reference();
    }

    ulong expr_op_eq(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);


        if (lhs.is_bool != rhs.is_bool) {
            error_arithmetic_op_requires_numerics("==");
        }

        /*
        Value lval, rval;

        version (MIXED_COMPARE) {
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
        } else {
            if (lhs.is_bool != rhs.is_bool) {
                error_mixed_comparison_types();
            }
            lval = lhs.value;
            rval = rhs.value;
        }
        */

        auto res = new Expression;
        res.value = current_builder.icmp_eq(lhs.value, rhs.value);
        res.is_bool = true;
        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        */
        return res.reference();
    }

    ulong expr_op_neq(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (lhs.is_bool || rhs.is_bool) {
            error_arithmetic_op_requires_numerics("!=");
        }

        auto res = new Expression;
        res.value = current_builder.icmp_ne(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_lt(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);


        if (lhs.is_bool || rhs.is_bool) {
            error_arithmetic_op_requires_numerics("<");
        }

        /*
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
        */

        auto res = new Expression;
        res.value = current_builder.icmp_slt(lhs.value, rhs.value);
        res.is_bool = true;
        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        */
        return res.reference();
    }

    ulong expr_op_gt(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        if (lhs.is_bool || rhs.is_bool) {
            error_arithmetic_op_requires_numerics(">");
        }

        /*
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
        */

        auto res = new Expression;
        res.value = current_builder.icmp_sgt(lhs.value, rhs.value);
        res.is_bool = true;
        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        */
        return res.reference();
    }

    ulong expr_op_lte(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);


        if (lhs.is_bool || rhs.is_bool) {
            error_arithmetic_op_requires_numerics("<=");
        }

        /*
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
        */

        auto res = new Expression;
        res.value = current_builder.icmp_slte(lhs.value, rhs.value);
        res.is_bool = true;
        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        */
        return res.reference();
    }

    ulong expr_op_gte(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);


        if (lhs.is_bool || rhs.is_bool) {
            error_arithmetic_op_requires_numerics(">=");
        }

        /*
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
        */

        auto res = new Expression;
        res.value = current_builder.icmp_sgte(lhs.value, rhs.value);
        res.is_bool = true;
        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        */
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
        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        */
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
        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        */
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
        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        */
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
        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        */
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
        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        if (rhs.const_is_sym !is null) {
            rhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 1);
        }
        */
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
        res.is_bool = true;
        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        */
        return res.reference();
    }

    ulong expr_op_binv(ulong lhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);

        if (lhs.is_bool) {
            error_arithmetic_op_requires_numerics("~");
        }

        auto res = new Expression;
        res.value = current_builder.binv(lhs.value);
        return res.reference();
    }

    ulong expr_op_neg(ulong lhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);

        if (lhs.is_bool) {
//            stderr.writeln("error: '-' must be used with a numeric value (line ",yylineno,")");
//            generic_error();
            error_arithmetic_op_requires_numerics("-");
        }
        auto res = new Expression;
        res.value = current_builder.neg(lhs.value);
        /*
        if (lhs.const_is_sym !is null) {
            lhs.const_is_sym.associated_const_usages[current_block] ~= new InstUsage(res.value, 0);
        }
        */

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
            fn.values[null] = current_module.add_function(text(ident), Type.function_type(numeric_type, [eh_ptr_type] ~ types));
            fn.arg_types = types;
        } else {
            if (fn.arg_types.length != p.values.length) {
//                stderr.writeln("error: incorrect number of parameters for function ",text(ident), " (line ",yylineno,")");
//                generic_error();
                error_inconsistent_function(text(ident));
            }
        }
        current_value = current_builder.call(fn.values[null], [current_eh] ~ p.values);

        /*
        foreach (i,s; p.const_syms) {
            if (s !is null) {
                s.associated_const_usages[current_block] ~= new InstUsage(current_value, i);
            }
        }
        */
    }

    /* Statements */

    void statement_assign(char *lhs, ulong rhs_ref) {
        auto sym = find_or_create_symbol(text(lhs));
        if (sym.type != SymbolType.VARIABLE && sym.type != SymbolType.NONE) {
            error_symbol_of_different_type(text(lhs));
        }
        if (sym.parent is null) {
            sym.parent = current_function;
        }
        auto rhs = Expression.lookup(rhs_ref);

        sym.type = SymbolType.VARIABLE;

        sym.parent = current_function;
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

//        auto syms1 = find_symbols_in_blocks(ifelse.during);
        if (nested_ifelse_ref == ulong.max) {
            phi_blocks = [ifelse.during, prev_block];
        } else {
            phi_blocks = [ifelse.during, nested.after];
        }
        auto syms1 = find_symbols_in_blocks(phi_blocks[0], phi_blocks[1]);

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

//        auto syms2 = find_symbols_in_block(ifelse.otherwise);
        phi_blocks[1] = prev_block;
        auto syms2 = find_symbols_in_blocks(ifelse.otherwise, prev_block);
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

        foreach (sym; SymbolTable.symbols) {
            if ((sym.type != SymbolType.VARIABLE && sym.type != SymbolType.CHANNEL) || sym.is_global)
                continue;

            Value p;
            if (sym.is_bool)
                p = current_builder.phi(bool_type);
            else
                p = current_builder.phi(numeric_type);

            p.add_incoming([sym.values[sym.last_block]], [loop.before]);
            sym.values[loop.test] = p;
            sym.last_block = loop.test;
        }

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

//        auto syms1 = find_symbols_in_block(loop.test);
//        auto syms2 = find_symbols_in_block(loop.during);
//        auto syms1 = find_symbols_in_blocks(loop.test, loop.during);
//        auto syms2 = find_symbols_in_blocks(loop.during, loop.after);

        auto syms = find_symbols_in_blocks(loop.test, loop.after);

        foreach (sym; syms) {
            if (loop.test in sym.values) {
                sym.values[loop.test].add_incoming([sym.values[sym.last_block]], [current_block]);
                sym.last_block = loop.test;
            }
        }

        /*
        Value[] phi_values;
        Value[Value] replacements;
        BasicBlock[] phi_blocks = [loop.before, current_block];
        current_builder.position_before(loop.test.first_instruction());

        foreach (sym; syms) {
            phi_values = [];
            foreach (bl; phi_blocks) {
                auto v = sym.values.get(bl, void_value);
                ulong min = 0;
                if (v == void_value) {
                    foreach (k, va; sym.values) {
                        if (k.index >= min && k.index < bl.index && va != void_value) {
                            min = k.index;
                            v = va;
                        }
                    }
                    if (v == void_value) {
                        warn_conditionally_defined(sym.ident);
                        continue;
                    }
                }
                phi_values ~= v;
            }
            Type t = null;
            foreach (v; phi_values) {
                if (t is null) {
                    t = v.type;
                } else if (!t.same(v.type)) {
                    warn_unexpected_error();
                    error_indeterminate_statement_type();
                }
            }
            Value newval = current_builder.make_phi(t, phi_values, phi_blocks);
            sym.values[loop.test] = newval;

            foreach (v; phi_values) {
                replacements[v] = newval;
            }

            sym.last_block = loop.test;
            sym.is_bool = t.same(bool_type);
        }
        */

        /*
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
        */

        /*
        Value[] phi_values;
        BasicBlock[] phi_blocks = [loop.before, loop.during];

        current_builder.position_before(loop.test.first_instruction());

        Value[Value] replacements;
        
        foreach (sym; syms1 ~ syms2) {
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
                        continue;
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
            sym.values[loop.during] = newval;
//            sym.values[loop.after] = sym.values[loop.test];
            sym.values[loop.test] = newval;
//            sym.values[loop.after] = newval;
            ulong min = 0;
            BasicBlock prev_block = null;
            foreach (bl, va; sym.values) {
                if (bl.index >= min && bl.index <= loop.before.index) {
                    min = bl.index;
                    prev_block = bl;
                }
            }
            replacements[sym.values[prev_block]] = newval;
            
            sym.last_block = loop.test;
            sym.is_bool = t.same(bool_type);

        }
        */

        /*
        current_builder.position_at_end(loop.after);
        phi_blocks = [loop.before, loop.test];

        foreach (sym; syms1 ~ syms2) {
            phi_values = [];
            foreach (bl; phi_blocks) {
                auto v = sym.values.get(bl, void_value);
                ulong min = 0;
                if (v == void_value) {
                    foreach (k, va; sym.values) {
                        if (k.index >= min && k.index <= loop.test.index && va != void_value) {
                            min = k.index;
                            v = va;
                        }
                    }
                    if (v == void_value) {
                        warn_conditionally_defined(sym.ident);
                        continue;
                    }
                }
                phi_values ~= v;
            }
            Type t = null;
            foreach (v; phi_values) {
                if (t is null) {
                    t = v.type;
                } else if (!t.same(v.type)) {
                    warn_unexpected_error();
                    error_indeterminate_statement_type();
                }
            }
            Value newval = current_builder.make_phi(t, phi_values, phi_blocks);
            sym.values[loop.after] = newval;
            ulong min = 0;
            BasicBlock prev_block = null;
            foreach (bl, va; sym.values) {
                if (bl.index >= min && bl.index <= loop.test.index) {
                    min = bl.index;
                    prev_block = bl;
                }
                replacements[sym.values[prev_block]] = newval;
                sym.last_block = loop.after;
                sym.is_bool = t.same(bool_type);
            }
        }
        */

        /*
        Value inst = loop.test.first_instruction();

        while (inst !is null) {

            if (!inst.is_phi()) {
                foreach (i; 0 .. inst.get_num_operands()) {
                    Value operand = inst.get_operand(i);
                    if (operand.is_const()) {
                        bool stop = false;
                        foreach (sym; syms) {
                            foreach (usage; sym.associated_const_usages.get(loop.test, [])) {
                                if (usage.inst.same(inst) && usage.pos == i) {
                                    inst.set_operand(i, sym.values[loop.test]);
                                    stop = true;
                                    break;
                                }
                            }
//                            if (stop)
//                                break;
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
                        bool stop = false;
                        foreach (sym; syms) {
                            foreach (usage; sym.associated_const_usages.get(loop.during, [])) {
                                if (usage.inst.same(inst) && usage.pos == i) {
                                    inst.set_operand(i, sym.values[loop.during]);
                                    stop = true;
                                    break;
                                }
                            }
//                            if (stop)
//                                break;
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
        */

        current_builder.position_at_end(loop.after);

        /*
        foreach (sym; syms2) {
            sym.values[loop.after] = sym.values[loop.during];
        }
        foreach (sym; syms1) {
            sym.values[loop.after] = sym.values[loop.test];
        }*/
        /*
        foreach (sym; syms) {
            sym.values[loop.after] = sym.values[loop.test];
            sym.last_block = loop.after;
        }
        */


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
            fn.values[null] = current_module.add_function(text(ident), Type.function_type(numeric_type, [eh_ptr_type]));
        } else {
            if (fn.arg_types.length != 0) {
//                stderr.writeln("error: cannot fork function with arguments ", text(ident), " (line ",yylineno,")");
//                generic_error();
                error_fork_function_with_arguments(text(ident));
            }
        }
        current_builder.call(fork_fn, [current_eh, fn.values[null]]);
        current_value = void_value;
    }

    void statement_sync(int read, int write) {
        foreach (sym; SymbolTable.symbols) {
            if (sym.is_global && sym.parent == current_function) {
                if (write) {
                    Value tmp;
                    if (sym.is_bool) {
                        tmp = current_builder.select(sym.values[current_block], true_numeric_value, false_numeric_value);
                    } else {
                        tmp = sym.values[current_block];
                    }
                    current_builder.store(tmp, sym.global_value);
                }
                if (read) {
                    sym.parent = null;
                }
            }
        }
    }

    /* Functions */

    Value make_eh(Value parent, string name) {
        auto eh = current_builder.alloca(eh_type);

        auto pptr = current_builder.struct_gep(eh, HandlerEntryField.PARENT);
        current_builder.store(parent, pptr);

        if (include_trace_names) {
            auto fnameptr = current_builder.struct_gep(eh, HandlerEntryField.NAME);
            auto fname = current_builder.global_string_ptr(name);
            current_builder.store(fname, fnameptr);
        }

        auto fptr = current_builder.struct_gep(eh, HandlerEntryField.FLAGS);
        current_builder.store(eh_default_flags, fptr);
        return eh;
    }

    void function_begin(char *ident, ulong args_ref, ulong attr) {
        auto args = Arguments.lookup(args_ref);
        auto fn = find_symbol(text(ident));
        if (fn is null) {
            fn = create_symbol(text(ident));
            if (attr & FunctionAttribute.HANDLER) {
                fn.is_handler = true;
                current_function = current_module.add_function(text(ident), Type.function_type(Type.void_type(), args.types));
            } else {
                current_function = current_module.add_function(text(ident), Type.function_type(numeric_type, [eh_ptr_type] ~ args.types));
            }
            fn.is_global = true;
            fn.type = SymbolType.FUNCTION;
            fn.values[null] = current_function;
            fn.arg_types = args.types;
        } else {
            if (fn.type != SymbolType.FUNCTION) {
                error_function_shadows_different_type(text(ident));
            }
            if (attr & FunctionAttribute.HANDLER && !fn.is_handler) {
                error_handler_attr_unexpected();
            }
            if (!(attr & FunctionAttribute.HANDLER) && fn.is_handler) {
                error_handler_attr_expected();
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

        current_function_has_handlers = [0,0,0];

        if (attr & FunctionAttribute.HANDLER) {
            current_eh = null;
        } else if (attr & FunctionAttribute.PARENTSCOPE) {
            current_eh = current_function.get_param(0);
        } else {
            current_eh = make_eh(current_function.get_param(0), text(ident));
        }

        if (attr & FunctionAttribute.ENTRY) {
            if (entry_function) {
                error_multiple_entry();
            }
            if (fn.arg_types.length != 0) {
                error_entry_with_arguments();
            }
            entry_function = Value.add_alias(current_module, Type.pointer_type(Type.function_type(numeric_type, [eh_ptr_type])), current_function, "___entry");
        } else {
            current_function.set_internal_linkage();
        }

        current_value = void_value;

        foreach (i, sym; args.symbols) {
            sym.values[current_block] = current_function.get_param(cast(uint)i+(fn.is_handler ? 0 : 1));
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
                Value tmp;
                if (sym.is_bool) {
                    tmp = current_builder.select(sym.values[sym.last_block], true_numeric_value, false_numeric_value);
                } else {
                    tmp = sym.values[sym.last_block];
                }
                current_builder.store(tmp, sym.global_value);
                sym.parent = null;
            }
        }

        if (current_function_has_handlers[HandlerNumber.ALWAYS]) {
            function_handler_check(HandlerNumber.ALWAYS);
        }
        if (current_function_has_handlers[HandlerNumber.SUCCESS]) {
            function_handler_check(HandlerNumber.SUCCESS);
        }

        if (current_function.type.get_return_type().get_return_type().same(Type.void_type())) {
            current_builder.ret_void();
        } else {
            current_builder.ret(current_value);
        }
        flush_symbols(current_function);

    }

    void function_handler_check(int type) {
        auto epflags = current_builder.struct_gep(current_eh, HandlerEntryField.FLAGS);
        auto flags = current_builder.load(epflags);
        auto bit = current_builder.and(flags, Value.create_const_int(numeric_type, (1 << type)));
        auto bb_handler = current_function.append_basic_block("handler_"~to!string(type));
        auto bb_posthandler = current_function.append_basic_block(null);
        auto cond = current_builder.icmp_ne(bit, false_numeric_value);
        auto br = current_builder.cond_br(cond, bb_handler, bb_posthandler);
        current_builder.position_at_end(bb_handler);
        auto epfn = current_builder.struct_gep(current_eh, HandlerEntryField.ALWAYS+type);
        auto fn = current_builder.load(epfn);
        current_builder.call(fn, []);
        current_builder.br(bb_posthandler);
        current_block = bb_posthandler;
        current_builder.position_at_end(bb_posthandler);
    }

    /* Attributes */

    ulong attribute_value(ulong old, char *ident) {
        ulong attr = function_attributes.get(text(ident), FunctionAttribute.NONEXISTANT);
        if (attr == FunctionAttribute.NONEXISTANT) {
            error_no_such_attribute(text(ident));
        }
        return old | attr;
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

    void global_create(char *ident, int value, int is_bool) {
        auto sym = create_symbol(text(ident));

        sym.is_global = true;
        sym.type = SymbolType.VARIABLE;
        sym.is_bool = is_bool != 0;

        sym.global_value = Value.create_global_variable(current_module, numeric_type, text(ident));
        sym.global_value.set_initializer(Value.create_const_int(numeric_type, value));
    }

    /* Scope */

    void statement_scope(int type, char *ident) {
        if (current_eh is null) {
            error_scope_forbidden();
        }
        current_value = void_value;
        auto fn = find_symbol(text(ident));
        if (fn is null) {
            fn = create_symbol(text(ident));
/*            fn.is_global = true; */
            fn.is_handler = true;
            fn.type = SymbolType.FUNCTION;
            if (type == HandlerNumber.FAILURE) {
                fn.values[null] = current_module.add_function(text(ident), Type.function_type(Type.void_type(), [ex_info_type]));
                fn.arg_types = [ex_info_type];
            } else {
                fn.values[null] = current_module.add_function(text(ident), Type.function_type(Type.void_type(), []));
                fn.arg_types = [];
            }
        } else {
            if (!fn.is_handler) {
                error_scope_not_handler();
            }
        }
        auto epflags = current_builder.struct_gep(current_eh, HandlerEntryField.FLAGS);
        auto flags = current_builder.load(epflags);
        auto newflags = current_builder.or(flags, Value.create_const_int(numeric_type, 1 << type));
        current_builder.store(newflags, epflags);
        auto fptr = current_builder.struct_gep(current_eh, HandlerEntryField.ALWAYS + type);
        current_function_has_handlers[type] = true;
        current_builder.store(fn.values[null], fptr);
    }

    /* $ Builtins */

    void statement_hidden_fail() {
        current_builder.call(fail_fn, [current_eh]);
        current_value = void_value;
    }

    void statement_hidden_trace() {
        current_builder.call(trace_fn, [current_eh]);
        current_value = void_value;
    }

    /* Channels */

    ulong expr_atom_channel_receive(char *ident) {
        auto chansym = find_symbol(text(ident));

        if (chansym is null) {
            error_undeclared_channel(text(ident));
        }
        if (chansym.type != SymbolType.CHANNEL) {
            error_requires_channel("receive");
        }
    }

    void statement_channel_open(char *ident) {
        auto chansym = find_or_create_symbol(text(ident));
        if (chansym.type == SymbolType.VARIABLE) {
            error_channel_previously_variable();
        }

        chansym.type = SymbolType.CHANNEL;
        current_value = void_value;
    }

    void statement_channel_close(char *ident) {
        auto chansym = find_symbol(text(ident));

        if (chansym is null) {
            error_undeclared_channel(text(ident));
        }

        if (chansym.type != SymbolType.CHANNEL) {
            error_requires_channel("close");
        }

        current_value = void_value;
    }

    void statement_channel_accept(char *comm_ident, char *list_ident) {
        auto commsym = find_or_create_symbol(text(comm_ident));

        if (commsym.type == SymbolType.VARIABLE) {
            error_channel_previously_variable();
        }

        commsym.type = SymbolType.CHANNEL;

        auto listsym = find_symbol(text(list_ident));
        if (listsym is null) {
            error_undeclared_channel(text(list_ident));
        }

        current_value = void_value;
    }

    void statement_channel_send(char *lhs, ulong rhs_ref) {
        auto chansym = find_symbol(text(lhs));
        auto rhs = Expression.lookup(rhs_ref);

        if (chansym.type != SymbolType.CHANNEL) {
            error_requires_channel("send");
        }

        current_value = rhs.value;
    }
}



