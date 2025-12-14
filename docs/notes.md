# Implementation Notes

- **Day 1**: O(n) pass over input; constant memory.
- **Day 2**: Generates repeated-digit candidates by prefix/multiplier; avoids range scans. O(d) per digit length; constant extra memory.
- **Day 3**: Part1 single pass. Part2 uses monotonic selection for top-12 digits per line. O(n) time, O(1) memory per line.
- **Day 4**: Grid stored in compact char array; adjacency counts in byte array. Queue-based removal updates neighbor counts; O(n) time, O(n) memory.
- **Day 5**: Ranges parsed, sorted, merged; membership via binary search (O(log r)). Part2 sums merged lengths. O(r log r) time, O(r) memory.
- **Day 6**: Parses column-separated vertical problems; Part1 left-to-right, Part2 right-to-left. O(hw) time to scan grid, O(hw) memory for padded grid.
- **Testing**: `tests/run_samples.sh` runs provided samples/edge cases for days 1–6 (`just test`).
