#!/bin/bash

cachegrind="valgrind --tool=cachegrind"

rm cachegrind/* -r
mkdir -p cachegrind

# Baselines
${cachegrind} --log-file="cachegrind/PoolLinBaseline.txt" ./Test "[Pool][SetupAlloc]"
${cachegrind} --log-file="cachegrind/StandLinBaseline.txt" ./Test "[Stand][SetupAlloc]"
${cachegrind} --log-file="cachegrind/PoolFragBaseline.txt" ./Test "[Pool][SetupAlloc],[Pool][SetupFrag]"
${cachegrind} --log-file="cachegrind/StandFragBaseline.txt" ./Test "[Stand][SetupAlloc],[Pool][SetupFrag]"

# Measurements
${cachegrind} --log-file="cachegrind/PoolLin.txt" ./Test "[Pool][SetupAlloc]", "Pool- Linear access"
${cachegrind} --log-file="cachegrind/StandLin.txt" ./Test "[Stand][SetupAlloc]", "Stand- Linear access"
${cachegrind} --log-file="cachegrind/PoolFrag.txt" ./Test "[Pool][SetupAlloc],[Pool][SetupFrag]", "Pool- Fragmented linear access"
${cachegrind} --log-file="cachegrind/StandFrag.txt" ./Test "[Stand][SetupAlloc],[Stand][SetupFrag]", "Stand- Fragmented linear access"
