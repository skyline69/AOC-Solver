# Implementation Notes

- **Day 1**: O(n) pass over input; constant memory.
- **Day 2**: Generates repeated-digit candidates by prefix/multiplier; avoids range scans. O(d) per digit length; constant extra memory.
- **Day 3**: Part1 single pass. Part2 uses monotonic selection for top-12 digits per line. O(n) time, O(1) memory per line.
- **Day 4**: Grid stored in compact char array; adjacency counts in byte array. Queue-based removal updates neighbor counts; O(n) time, O(n) memory.
- **Day 5**: Ranges parsed, sorted, merged; membership via binary search (O(log r)). Part2 sums merged lengths. O(r log r) time, O(r) memory.
- **Day 6**: Parses column-separated vertical problems; Part1 left-to-right, Part2 right-to-left. O(hw) time to scan grid, O(hw) memory for padded grid.
- **Day 7**: Beam splitter simulation downward; track active columns per row. O(hw) time, O(w) memory.
- **Day 8**: Part1 picks k-shortest pairs (k=1000; 10 for sample) via fixed-size max-heap; unions to form circuits; product of top 3 component sizes. Part2 uses O(n^2) Prim MST (no edge storage) and returns the last-connecting edge's X-product. Memory: O(n).
- **Day 9**: Part1: max rectangle between any red corners. Part2: fill orthogonal polygon (red boundary + interior green) via scanline intervals; compress identical rows and use binary search for coverage. Roughly O(r^2 * h') with row compression, memory O(h + r).
- **Day 10**: Part1 solves GF(2) toggle system via Gaussian elimination (enumerates free vars up to a cutoff). Part2 solves `Ax=t` with `x>=0` integers via branch-and-bound on an LP relaxation (two-phase simplex; pivots artificials out) and validates integer candidates against constraints.
- **Testing**: `tests/run_samples.sh` runs provided samples/edge cases for days 1–10 (`just test`).
