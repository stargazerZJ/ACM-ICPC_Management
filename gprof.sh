#!/bin/zsh

# compile, show warnings
#g++ ./src/main.cpp -O2 -pg -g -o ./cmake-build-debug/GP_ACM_ICPC_Management
# compile, ignore warnings
g++ ./src/main.cpp -O2 -pg -o ./cmake-build-debug/GP_ACM_ICPC_Management -w

id=bigger
./cmake-build-debug/GP_ACM_ICPC_Management < ./data/$id.in > ./data/output.txt
diff -uZB ./data/$id.out ./data/output.txt > ./data/diff.txt
if [ $? -ne 0 ]
then
    echo "test case $id failed"
    # stop the script if one test case failed
    exit 1
fi

gprof ./cmake-build-debug/GP_ACM_ICPC_Management gmon.out > ./gprof.txt