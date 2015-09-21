import std.stdio;
import std.string;
import std.conv;

class Manifest {
    entry[string] entries;

    struct call_info {
        bool accepted;
        bool boolean_result;
    }

    struct entry {
        uint index;
        call_info[string] accepted_calls;
        bool accept_string, accept_value;

        bool syscall_allowed(string call) {
            if (!accepted_calls.get(call, call_info(false)).accepted) {
                return false;
            }
            return true;
        }

        bool syscall_arguments_allowed(bool has_string_arg, bool has_value_arg) {
            if ((has_string_arg && !accept_string) || (has_value_arg && !accept_value)) {
                return false;
            }
            return true;
        }

        bool syscall_boolean(string call) {
            return accepted_calls.get(call, call_info(false,false)).boolean_result;
        }
    }

    static Manifest load(File f) {
        auto man = new Manifest;
        ulong linenum = 0;
        foreach (ln; f.byLine) {
            entry ent;
            linenum++;
            ln = stripLeft(ln);
            if (ln.length == 0)
                continue;
            string qident;
            foreach (ch; ln) {
                if (ch == ' ' || ch == '\t') {
                    break;
                }
                qident ~= ch;
            }
            ln = stripLeft(ln[qident.length..$]);
            string a, b;
            foreach (ch; ln) {
                if (ch == ' ' || ch == '\t') {
                    break;
                }
                a ~= ch;
            }
            ln = stripLeft(ln[a.length..$]);
            foreach (ch; ln) {
                if (ch == ' ' || ch == '\t') {
                    break;
                }
                b ~= ch;
            }
            ln = stripLeft(ln[b.length..$]);
            if (ln.length > 0) {
                ulong n = 0;
                string[] attributes = [""];
                foreach (ch; ln) {
                    if (ch == ',') {
                        attributes ~= [""];
                    } else if (ch == ' ' || ch == '\t') {
                        //ignore
                    } else {
                        attributes[$-1] ~= ch;
                    }
                }
                foreach (attr; attributes) {
                    switch (attr) {
                        default:
                            stderr.writeln("Manifest: unknown attribute '", attr, "' in entry (line ",linenum,")");
                            break;
                        case "r":
                            ent.accepted_calls["read"] = call_info(true);
                            break;
                        case "w":
                            ent.accepted_calls["write"] = call_info(true);
                            break;
                        case "rw":
                            ent.accepted_calls["read"] = call_info(true);
                            ent.accepted_calls["write"] = call_info(true);
                            break;
                        case "s":
                        case "sv":
                            ent.accept_string = true;
                            ent.accept_value = true;
                            break;
                        case "v":
                        case "!sv":
                            ent.accept_string = false;
                            ent.accept_value = true;
                            break;
                        case "s!v":
                            ent.accept_string = true;
                            ent.accept_value = false;
                            break;
                        case "b":
                            ent.accepted_calls["block"] = call_info(true);
                            break;
                        case "rB":
                            ent.accepted_calls["read"] = call_info(true,true);
                            break;
                        case "wB":
                            ent.accepted_calls["write"] = call_info(true,true);
                            break;
                        case "rwB":
                            ent.accepted_calls["read"] = call_info(true,true);
                            break;
                        case "bB":
                            ent.accepted_calls["block"] = call_info(true,true);
                    }
                }
            }
            if (qident in man.entries) {
                stderr.writeln("Duplicate manifest entry (line ",linenum,")");
                continue;
            }
            try {
                ent.index = ((to!uint(b) & 0xFFFF) << 16) | (to!uint(a) & 0xFFFF);
            } catch (Exception e) {
                stderr.writeln("Invalid manifest entry (line ",linenum,")");
            }
            man.entries[qident] = ent;
        }
        return man;
    }
}
