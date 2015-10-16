module util;

import core.exception;
import std.string;

import errors;

extern (C) {
    __gshared int error_occured = 0;
}

mixin template ReferenceHandler() {
    private static typeof(this)[] references;

    private bool has_refid = false;
    private ulong refid;

    ulong reference(this T)() {
        if (!has_refid) {
            references ~= this;
            has_refid = true;
            refid = references.length - 1;
        }
        return refid;
    }

    static typeof(this) lookup(ulong id) {
        if (id >= references.length)
            return null;
        else
            return references[id];
    }
}

long dehexify(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    error_not_hex(c);
    return 0;
}

extern (C) {
immutable(char) *escape_string(char *s) {
    string escaped;
    while (*s) {
        if (*s == '\\') {
            s++;
            switch (*s) {
                default:
                    escaped ~= "\\" ~ *s;
//                    error_unknown_escape_sequence(*s);
                    break;
                case 'n':
                    escaped ~= '\n';
                    break;
                case 'r':
                    escaped ~= '\r';
                    break;
                case 't':
                    escaped ~= '\t';
                    break;
                case 'e':
                    escaped ~= '\x1B';
                    break;
                case 'b':
                    escaped ~= '\x08';
                    break;
                case 'x':
                    if (s[1] && s[2]) {
                        escaped ~= cast(char)((dehexify(s[1]) << 4) | dehexify(s[2])); 
                        s += 2;
                    } else {
                        error_incomplete_escape();
                    }
                case '\\':
                    escaped ~= '\\';
                    break;
                case '"':
                    escaped ~= '"';
                    break;
            }
        } else {
            escaped ~= *s;
        }
        s++;
    }
    return toStringz(escaped);
}
}

