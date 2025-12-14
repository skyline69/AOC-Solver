#!/usr/bin/env bash
set -euo pipefail
ROOT="$(dirname "$0")/.."
BIN="$ROOT/bin/aoc"

run_line() {
  day=$1; part=$2; input=$3; expected=$4
  line=$($BIN --day "$day" --part "$part" --input "$input" | grep "[Part $part]")
  out=$(echo "$line" | awk '{print $3}')
  if [[ "$out" != "$expected" ]]; then
    echo "FAIL day=$day part=$part expected=$expected got=$out" >&2
    return 1
  fi
  echo "ok day=$day part=$part=$out"
}

# Day 1 simple sample
run_line 1 1 "$ROOT/tests/day1_sample.txt" 0
run_line 1 2 "$ROOT/tests/day1_sample.txt" 0

# Day 2 placeholder (requires user input); skip if missing
if [[ -f "$ROOT/input.txt" ]]; then
  d2p1=$($BIN --day 2 --part 1 --input "$ROOT/input.txt" | grep "[Part 1]" | awk '{print $3}')
  d2p2=$($BIN --day 2 --part 2 --input "$ROOT/input.txt" | grep "[Part 2]" | awk '{print $3}')
  run_line 2 1 "$ROOT/input.txt" "$d2p1"
  run_line 2 2 "$ROOT/input.txt" "$d2p2"
fi

# Day 3 sample
run_line 3 1 "$ROOT/tests/day3_sample.txt" 357
run_line 3 2 "$ROOT/tests/day3_sample.txt" 3121910778619
run_line 3 2 "$ROOT/tests/day3_edge_short.txt" 1123456789032

# Day 4 sample
run_line 4 1 "$ROOT/tests/day4_sample.txt" 13
run_line 4 2 "$ROOT/tests/day4_sample.txt" 43
run_line 4 1 "$ROOT/tests/day4_edge_small.txt" 2

# Day 5 sample
run_line 5 1 "$ROOT/tests/day5_sample.txt" 3
run_line 5 2 "$ROOT/tests/day5_sample.txt" 14
run_line 5 1 "$ROOT/tests/day5_edge_ranges.txt" 6
run_line 5 2 "$ROOT/tests/day5_edge_ranges.txt" 6

# Day 6 sample
run_line 6 1 "$ROOT/tests/day6_sample.txt" 4277556
run_line 6 2 "$ROOT/tests/day6_sample.txt" 3263827

# Day 7 sample
run_line 7 1 "$ROOT/tests/day7_sample.txt" 21
run_line 7 2 "$ROOT/tests/day7_sample.txt" 40

# Day 8 sample (uses 10 edges)
run_line 8 1 "$ROOT/tests/day8_sample.txt" 40
run_line 8 2 "$ROOT/tests/day8_sample.txt" 25272

# Day 9 sample
run_line 9 1 "$ROOT/tests/day9_sample.txt" 50
run_line 9 2 "$ROOT/tests/day9_sample.txt" 24

# Day 10 sample
run_line 10 1 "$ROOT/tests/day10_sample.txt" 7
run_line 10 2 "$ROOT/tests/day10_sample.txt" 33
