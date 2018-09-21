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

usage()
{
    echo
    echo "Usage: $0 n"
    echo
    echo "Where n is the desired task number in range [1..5]"
    echo
    exit 1
}

if [ "$#" -lt "1" ]; then
    echo "Error: task number not specified!"
    usage
fi

if [[ ! "$1" =~ ^[0-9]+$ ]]; then
    echo "Error: argument is not a number"
    usage
fi

if [ "$1" -lt "1" -o "$1" -gt "5" ]; then
    echo "Error: argument must be a number in range [1..5]"
    usage
fi

taskNumber=$1
taskNumber=$((taskNumber+0))

let count=0
let all=0

mkdir -p ./output

for test_file in $(find ./tests -type f -name "*.txt" | sort); do
    all=$((all+1))
    name=`basename ${test_file} .txt`
    expected_file=${test_file}.expected${taskNumber}
    output_file=./output/${name}.output
    diff_file=./output/${name}.diff
    ./a.out ${taskNumber} < ${test_file} > ${output_file}
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
echo "Passed $count tests out of $all for task ${taskNumber}"
echo

rmdir ./output
