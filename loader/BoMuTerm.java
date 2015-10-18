import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;
import java.util.ArrayList;

public class BoMuTerm {
    private static Process term;

    public static InputStream stdout, stderr;
    public static OutputStream stdin;

    static void openTerminal(String... args) throws IOException {
        ArrayList<String> al = new ArrayList<String>();
        
        al.add("./term.py");
        for (String s : args) al.add(s);

        ProcessBuilder pb = new ProcessBuilder(al);
        pb.redirectError(ProcessBuilder.Redirect.INHERIT);
        term = pb.start();

        stdout = term.getInputStream();
        stderr = term.getErrorStream();
        stdin = term.getOutputStream();
    }

    static void send(String s) throws IOException {
        stdin.write(s.getBytes("ASCII"));
        stdin.flush();
    }

    static void sendExit() throws IOException {
        stdin.write(0x18);
        stdin.flush();
    }

    static void sendLoad() throws IOException {
        stdin.write(0x10);
        stdin.flush();
    }

    public static void main(String[] args) {
        try {
            openTerminal("-", "-x");

            send("RUN");

            while (true) {
                if (stdout.available() > 0) {
                    System.out.print((char)stdout.read());
                }
            }
        } catch (IOException e) {
            System.out.println(e);
        }
    }
}

