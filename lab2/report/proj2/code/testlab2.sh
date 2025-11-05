#!/bin/bash

# examples
# cpus=("o3")
# mems=("DDR3")
#bps=("tournament" "bimode" "local")
#bps=("simple")
bps=("local")
exes=("branch_optimize")

# for c in ${cpus[@]}; do
# for m in ${mems[@]}; do
for e in ${exes[@]}; do
for b in ${bps[@]}; do
    # set output dir, name it however you like
    build/ARM/gem5.opt -d lab2/part4/optimized$e$b \
    configs/proj2/simple.py tests/labexe/$e \
    --bp $b &
    # add `&` after the command to let it run in background
done
done
