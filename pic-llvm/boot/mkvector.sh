#!/bin/bash

SRCDIR=$(dirname ${BASH_SOURCE[0]})
GENSRCDIR=$(dirname ${BASH_SOURCE[0]})/generated

mkdir -p $GENSRCDIR

function mkvector () {
    sed 's/NUM/'$1'/g' $SRCDIR/vector.template > $GENSRCDIR/vector"$1".S
}

mkvector 0
mkvector 1
mkvector 2
mkvector 3
mkvector 4
mkvector 5
mkvector 6
mkvector 7
mkvector 8
mkvector 9
mkvector 10
mkvector 11
mkvector 12
mkvector 13
mkvector 14
mkvector 15
mkvector 16
mkvector 17
mkvector 18
mkvector 19
mkvector 20
mkvector 21
mkvector 22
mkvector 23
mkvector 24
mkvector 25
mkvector 26
mkvector 27
mkvector 28
mkvector 29
mkvector 30
mkvector 31
mkvector 32
mkvector 33
mkvector 34
mkvector 35
mkvector 36
mkvector 37
mkvector 38
mkvector 39
mkvector 40
mkvector 41
mkvector 42
mkvector 43

