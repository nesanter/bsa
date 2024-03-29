function @entry top() {
    [write "\e[2JBasic I/O Interactive Tester\r\n", .console];
    while {
        [write "> ", .console];
        c = [block .console.rx.wait];
        [write c, .console.tx];
        [write "\r\n", .console];
        c != 'q';
    } do {
        if (c == 'l') {
            led_tester();
        } else if (c == 's') {
            switch_tester();
        } else if (c == 'd') {
            ldr_tester();
        } else if (c == 'p') {
            piezo_tester();
        } else {
            [write "(expected [l]ed, [s]witch, l[d]r, [p]iezo, or [q]uit)\r\n", .console];
            ;
        }
    }
}

function led_tester() {
    while {
        [write "led> ", .console];
        c = [block .console.rx.wait];
        [write c, .console.tx];
        [write "\r\n", .console];
        c != 'q';
    } do {
        if (c >= '0' and c <= '7') {
            [write c - '0', .led.select];
            [write [read .led] ^ 1, .led];
        } else {
            [write "(expected 0-7, [q]uit)\r\n", .console];
            ;
        }
    }
}

function switch_tester() {
    while {
        [write "switch> ", .console];
        c = [block .console.rx.wait];
        [write c, .console.tx];
        [write "\r\n", .console];
        c != 'q';
    } do {
        if (c >= '0' and c <= '3') {
            [write c - '0', .sw.select];
            if ([read .sw] is true) {
                [write "high\r\n", .console];
            } else {
                [write "low\r\n", .console];
            }
            ;
        } else if (c == 'e') {
            [write "edge>", .console];
            e = [block .console.rx.wait];
            [write e, .console.tx];
            [write "\r\n", .console];
            if (e == 'c') {
                s = [read .sw.edge];
                if (s == 0) {
                    [write "high->low\r\n", .console];
                } else if (s == 1) {
                    [write "low->high\r\n", .console];
                } else if (s == 2) {
                    [write "any edge\r\n", .console];
                } else {
                    [write "unknown edge\r\n", .console];
                }
                ;
            } else if (e == 'l') {
                [write 0, .sw.edge];
            } else if (e == 'h') {
                [write 1, .sw.edge];
            } else if (e == 'b') {
                [write 2, .sw.edge];
            } else {
                [write "(expected [c]urrent, [h]igh, [l]ow, or [b]oth)", .console];
                ;
            }
            ;
        } else if (c == 'w') {
            [block .sw.wait];
        } else {
            [write "(expected 0-3, [e]dge, [w]ait, [q]uit)\r\n", .console];
            ;
        }
    }
}

function ldr_tester() {
    [write true, .ldr.enable];
    while {
        [write "ldr> ", .console];
        c = [block .console.rx.wait];
        [write c, .console.tx];
        [write "\r\n", .console];
        c != 'q';
    } do {
        if (c == 'r') {
            [write [read .ldr], .console];
        } else {
            [write "(expected [r]ead or [q]uit)\r\n", .console];
            ;
        }
    }
}

function piezo_tester() {
    [write true, .piezo.enable];
    while {
        [write "piezo> ", .console];
        c = [block .console.rx.wait];
        [write c, .console.tx];
        [write "\r\n", .console];
        c != 'q';
    } do {
        if (c == 'f') {
            n = get_number();
            if (n < 2000 or n > 65535) {
                [write "(outside valid range of [2000,65535])\r\n", .console];
            } else {
                [write 0, .timer.select];
                [write n, .timer.period];
                [write "set frequency to ", .console];
                [write n, .console];
                [write "\r\n", .console];
            }
        } else if (c == 'p') {
            [write true, .piezo.active];
            yield for 1500;
            [write false, .piezo.active];
        } else if (c >= '0' and c <= '7') {
            [write 0, .timer.select];
            [write c - '0', .timer.prescaler];
            false;
        } else {
            [write "(expected [0-9] or [q]uit)\r\n", .console];
            ;
        }
    }
}

function get_number() {
    n = 0;
    while {
        c = [block .console.rx.wait];
        [write c, .console.tx];
        c >= '0' and c <= '9';
    } do {
        n = 10 * n + (c - '0');
    }
    n;
}
