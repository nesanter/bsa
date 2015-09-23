// function guess mismatch with decl

function @entry foo() {
    bar(8, 9, 10);
}

function bar(x, y, z, w) {
    ;
}
