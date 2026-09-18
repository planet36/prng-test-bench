#!/usr/bin/bash
# SPDX-FileCopyrightText: Steven Ward
# SPDX-License-Identifier: MPL-2.0

# Usage: run-benchmarks.bash
# Run all *-benchmark executables in the current directory, save raw results to
# results/, and print sorted median summaries.

export LC_ALL=C

NUM_THREADS="$(nproc --ignore=1)" # Use N-1 threads
#NUM_THREADS=1
export NUM_THREADS

# Should be an odd number for simpler median
BENCHMARK_REPS=${BENCHMARK_REPS:-5}

OUTPUT_DIR=results

mkdir --verbose --parents -- "$OUTPUT_DIR" || exit

shopt -s nullglob

for PROGRAM in *-benchmark
do
    test -x "$PROGRAM" || continue

    echo "# $PROGRAM"
    echo

    "${PIN_CMD[@]}" ./"$PROGRAM" \
        --benchmark_enable_random_interleaving=true \
        --benchmark_repetitions="$BENCHMARK_REPS" \
        --benchmark_report_aggregates_only=true \
        --benchmark_out_format=json \
        --benchmark_out="${OUTPUT_DIR}/${PROGRAM}.json" || exit

    echo
    echo "## Sorted and filtered results"
    echo

    jq --from-file "${OUTPUT_DIR}/filter-benchmark-results.jq" --raw-output "${OUTPUT_DIR}/${PROGRAM}.json" | sort > "${OUTPUT_DIR}/${PROGRAM}.txt" || exit

    sort -r -k 2 -g -- "${OUTPUT_DIR}/${PROGRAM}.txt" | column --table || exit

    echo "________________________________________________________________________________"
    echo
done
