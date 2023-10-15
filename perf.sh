#!/bin/bash

EXEC_PATH=./cmake-build-debug/ACM_ICPC_Management
echo "executable file path: $EXEC_PATH"
fl_dir=/home/zj/ProjectsU/FlameGraph
SVG_PATH=perf.svg
INPUT_FILE=./data/bigger.in
OUTPUT_FILE=./data/output.txt
input=${INPUT_FILE:-/dev/null}
output=${OUTPUT_FILE:-/dev/null}
echo "input: $input, output: $output"

# time "$EXEC_PATH" <"$input" >"$output"

"$EXEC_PATH" <"$input" >"$output" &
PID=$!
perf record -F 1000 -p $PID -g -- sleep 1
if [ $? -ne 0 ]; then
  echo "The program exited before perf is ready to record. Try with a bigger input." >&2
fi
kill $PID > /dev/null 2>&1
sleep 0.1
kill -9 $PID > /dev/null 2>&1
"$fl_dir/flamegraph.pl" <("$fl_dir/stackcollapse-perf.pl" <(perf script)) > "$SVG_PATH"
if [ $? -ne 0 ]; then
  echo "Flame graph generation failed." >&2
fi
rm -f perf.data