#!/bin/bash

make tests

echo "RUNNING TESTS"

for TEST in *_test
do
    echo "RUNNING $TEST"
    ./$TEST
    if [[ $? -ne 0 ]]
    then
        echo "TEST $TEST FAILED"
        exit 1
    fi
done

echo "ALL TESTS PASSED"
