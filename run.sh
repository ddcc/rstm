#!/bin/bash

set -e

if [ $# -lt 1 ]; then
  echo "$0 <threads>"
  exit
fi

declare -a RUN=("./build/bench/CounterBenchSSB64 -p"
                "./build/bench/DisjointBenchSSB64 -p"
                "./build/bench/DListBenchSSB64 -p"
                "./build/bench/ForestBenchSSB64 -p"
                "./build/bench/HashBenchSSB64 -p"
                "./build/bench/ListBenchSSB64 -p"
                "./build/bench/MCASBenchSSB64 -p"
                "./build/bench/ReadNWrite1BenchSSB64 -p"
                "./build/bench/ReadWriteNBenchSSB64 -p"
                "./build/bench/TreeBenchSSB64 -p"
                "./build/bench/TreeOverwriteBenchSSB64 -p"
                "./build/bench/TypeTestSSB64 -p"
                "./build/bench/WWPathologyBenchSSB64 -p")

for i in "${RUN[@]}"; do
  declare -a CMD=($i$1)
  echo "Executing: ${CMD[@]}"
  FILENAME="$(basename ${CMD[0]})"

  while [ -f "${FILENAME}.log" ]; do
    FILENAME+="0"
  done
  FILENAME+=".log"

  hostname > "${FILENAME}"
  STM_STATS=1 timeout --foreground --preserve-status 10m ${CMD[@]} >> "${FILENAME}" 2>&1
done
