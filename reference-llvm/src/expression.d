import llvmd.core;
import util;

import std.conv;

Builder current_builder;
BasicBlock current_block;

class Expression {
    mixin ReferenceHandler;
    Value value;
}

extern (C) {
    ulong expr_atom_ident(char *s) {
        auto sym = find_symbol(text(s));
        if (sym is null) {
            error_occured++;
            return ulong.max;
        }
        return sym.reference();
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
        res.value = current_builder.icmp_lt(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_gt(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto res = new Expression;
        res.value = current_builder.icmp_gt(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_lte(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto res = new Expression;
        res.value = current_builder.icmp_lte(lhs.value, rhs.value);

        return res.reference();
    }

    ulong expr_op_gte(ulong lhs_ref, ulong rhs_ref) {
        auto lhs = Expression.lookup(lhs_ref);
        auto rhs = Expression.lookup(rhs_ref);

        auto res = new Expression;
        res.value = current_builder.icmp_gte(lhs.value, rhs.value);

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

    ulong expr_op_div(ulong lhs_ref, ulong rhs_ref) {
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



}



