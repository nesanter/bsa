// not @handler

function bar() {
    ;
}

function @entry foo() {
    scope (always) bar;
}
