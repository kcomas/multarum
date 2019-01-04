#!/bin/sh

make tests

bold() {
    echo "\e[1m$1\e[0m"
}

bold_blue() {
    echo "\e[1;34m$1\e[0m"
}

bold_green() {
    echo "\e[1;32m$1\e[0m"
}

bold_red() {
    echo "\e[1;31m$1\e[0m"
}

bold "RUNNING TESTS"

for TEST in *_test
do
    bold_blue "RUNNING $TEST"
    ./$TEST
    if [ $? -ne 0 ]
    then
        bold_red "TEST $TEST FAILED"
        exit 1
    fi
    valgrind --leak-check=full --error-exitcode=12 ./$TEST
    if [ $? -eq 12 ]
    then
        bold_red "TEST $TEST HAS MEMORY ERRORS"
        exit 1
    fi
done

bold_green "ALL TESTS PASSED"
