#!/bin/bash

# examples
cpus=("simple" "minor" "o3")
mems=("DDR3" "DDR4" "DDR5")
exes=("shell_sort" "gemm" "spfa" "binary_search")

for c in ${cpus[@]}; do
for m in ${mems[@]}; do
for e in ${exes[@]}; do
    # set output dir, name it however you like
    build/ARM/gem5.opt -d lab1$e$c$m \
    configs/proj1/simple.py tests/labexe/$e \
    --cpu $c --mem $m &
    # add `&` after the command to let it run in background
done
done
done
