// bad use of @handler

function @entry foo() {
    bar();
}

function @handler bar() {
    ;
}
