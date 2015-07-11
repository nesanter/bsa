module util;

import core.exception;

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


