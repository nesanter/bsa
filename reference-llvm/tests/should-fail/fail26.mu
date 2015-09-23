// no args for @handler w/ always

function @entry foo() {
    scope (always) bar;
}

function @handler bar(x) {
    ;
}
