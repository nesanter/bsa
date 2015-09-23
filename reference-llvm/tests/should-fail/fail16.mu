// function decl doesn't match later use

function bar(x, y, z, w) {
    ;
}

function @entry foo() {
    bar(8, 9, 10);
}
