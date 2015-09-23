// conditionally declared variable

function @entry foo() {
    if (false) {
        x = 7;
    }
    x;
}
