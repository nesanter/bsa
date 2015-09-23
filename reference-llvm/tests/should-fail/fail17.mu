// ambiguously boolean

function @entry foo() {
    if (true) {
        true;
    } else {
        7;
    }
}
