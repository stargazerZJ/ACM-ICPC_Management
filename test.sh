#!/bin/zsh

# the command to test one test case is
# id=1 && ./cmake-build-debug/ACM_ICPC_Management < ./data/$id.in > ./data/output.txt && diff -uZB ./data/$id.out ./data/output.txt > ./data/diff.txt

# the names of the test cases are:
# 1.in, 2.in, 3.in, 4.in, 5.in, 6.in, 7.in, small.in, big.in, bigger.in, error.in, test.in

# now test all of them
for id in 1 2 3 4 5 6 7 small big bigger error test
do
    ./cmake-build-debug/ACM_ICPC_Management < ./data/$id.in > ./data/output.txt
    diff -uZB ./data/$id.out ./data/output.txt > ./data/diff.txt
    if [ $? -eq 0 ]
    then
        echo "test case $id passed"
    else
        echo "test case $id failed"
        # stop the script if one test case failed
        exit 1
    fi
done