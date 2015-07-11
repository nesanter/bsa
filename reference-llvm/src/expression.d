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

class Expression {
    mixin ReferenceHandler;
    Value value;
}

class Arguments {
    mixin ReferenceHandler;
    Type[] types;
    Symbol[] symbols;
}

class IfElse {
    mixin ReferenceHandler;

    bool empty = false;
    BasicBlock before, during, otherwise, after;

    Value during_value, otherwise_value;
}

void init() {
    current_module = Module.create_with_name("main_module");
    current_builder = Builder.create();
    numeric_type = Type.int_type(4);
    object_type = Type.struct_type([], false);
    void_value = Value.create_const_int(numeric_type, 0);
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
        res.value = sym.value;
        return res.reference();
    }

    ulong expr_atom_numeric(ulong n) {
        auto res = new Expression;
        res.value = Value.create_const_int(Type.int_type(4), n);

        return res.reference();
    }

    ulong expr_op_lor(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto a = current_builder.icmp_ne(lhs.value, Value.create_const_int(Type.int_type(4), 0));
        auto b = current_builder.icmp_ne(rhs.value, Value.create_const_int(Type.int_type(4), 0));

        auto res = new Expression;
        res.value = current_builder.select(a, Value.create_const_int(Type.int_type(4), 1), b);

        return res.reference();
    }

    ulong expr_op_lxor(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto a = current_builder.icmp_ne(lhs.value, Value.create_const_int(Type.int_type(4), 0));
        auto b = current_builder.icmp_ne(rhs.value, Value.create_const_int(Type.int_type(4), 0));

        auto res = new Expression;
        res.value = current_builder.icmp_ne(a, b);

        return res.reference();
    }

    ulong expr_op_land(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto a = current_builder.icmp_ne(lhs.value, Value.create_const_int(Type.int_type(4), 0));
        auto b = current_builder.icmp_ne(lhs.value, Value.create_const_int(Type.int_type(4), 0));

        auto res = new Expression;
        res.value = current_builder.select(a, b, Value.create_const_int(Type.int_type(4), 0));

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
        res.value = current_builder.icmp_eq(lhs.value, Value.create_const_int(Type.int_type(4), 0));

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


    /* Statements */

    void statement_assign(char *lhs, ulong rhs_ref) {
        auto sym = find_or_create_symbol(text(lhs));
        auto rhs = Expression.lookup(rhs_ref);

        sym.type = SymbolType.VARIABLE;
        sym.value = rhs.value;

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

        ifelse.before = current_block;
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
        current_block = ifelse.otherwise;
        ifelse.during_value = current_value;
        current_value = void_value;
    }

    void statement_if_end(ulong ifelse_ref) {
        auto ifelse = IfElse.lookup(ifelse_ref);
        ifelse.otherwise_value = current_value;
        current_builder.br(ifelse.after);
        current_builder.position_at_end(ifelse.after);
        current_block = ifelse.after;
        current_value = current_builder.make_phi(numeric_type,
                [ifelse.during_value, ifelse.otherwise_value],
                [ifelse.during, ifelse.otherwise]);
    }

    /* Functions */

    void function_begin(char *ident, ulong args_ref, int returns_object) {
        auto args = Arguments.lookup(args_ref);
        current_function = current_module.add_function(text(ident), Type.function_type(Type.int_type(4), args.types));
        current_block = current_function.append_basic_block("entry");
        current_builder.position_at_end(current_block);

        current_value = void_value;

        foreach (i, sym; args.symbols) {
            sym.value = current_function.get_param(cast(uint)i);
        }
    }

    void function_end() {
        current_builder.ret(current_value);
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
            a.types ~= Type.int_type(4);
            sym.type = SymbolType.VARIABLE;
        }
        a.symbols ~= sym;
        return a.reference();
    }

    ulong args_add(ulong args_ref, char *ident, int is_object) {
        auto a = Arguments.lookup(args_ref);
        auto sym = create_symbol(text(ident));
        if (sym is null) {
            stderr.writeln("error: ", text(ident), " shadows");
            generic_error();
        }
        if (is_object) {
            a.types ~= object_type;
            sym.type = SymbolType.OBJECT;
        } else {
            a.types ~= Type.int_type(4);
            sym.type = SymbolType.VARIABLE;
        }
        a.symbols ~= sym;
        return a.reference();
    }





}



