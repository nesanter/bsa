import std.stdio;
import std.string;
import std.conv;

class Manifest {
    ushort[2][string] entries;

    static Manifest load(File f) {
        auto man = new Manifest;
        ulong linenum = 0;
        foreach (ln; f.byLine) {
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
                stderr.writeln("Malformed manifest entry (line ",linenum,")");
                continue;
            }
            if (qident in man.entries) {
                stderr.writeln("Duplicate manifest entry (line ",linenum,")");
                continue;
            }
            try {
                man.entries[qident] = [to!ushort(a), to!ushort(b)];
            } catch (Exception e) {
                stderr.writeln("Invalid manifest entry (line ",linenum,")");
            }
        }
        return man;
    }
}
