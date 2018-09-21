#!/bin/bash

if [ ! -d "./tests" ]; then
    echo "Error: tests directory not found!"
    exit 1
fi

if [ ! -e "./a.out" ]; then
    echo "Error: a.out not found!"
    exit 1
fi

if [ ! -x "./a.out" ]; then
    echo "Error: a.out not executable!"
    exit 1
fi

let count=0
let all=0

mkdir -p ./output

for test_file in $(find ./tests -type f -name "*.txt" | sort); do
    all=$((all+1))
    name=`basename ${test_file} .txt`
    expected_file=${test_file}.expected
    output_file=./output/${name}.output
    diff_file=./output/${name}.diff
    ./a.out < ${test_file} > ${output_file}
    diff -Bw ${expected_file} ${output_file} > ${diff_file}
    echo
    if [ -s ${diff_file} ]; then
        echo "${name}: Output does not match expected:"
        echo "--------------------------------------------------------"
        cat ${diff_file}
    else
        count=$((count+1))
        echo "${name}: OK"
    fi
    echo "========================================================"
    rm -f ${output_file}
    rm -f ${diff_file}
done

echo
echo "Passed $count tests out of $all"
echo

rmdir ./output
