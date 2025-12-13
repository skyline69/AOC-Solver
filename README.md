# aoc

Small C CLI for solving AOC quests

## Build & Run
- Debug (ASan): `just run`  
  - Note: AddressSanitizer may not run in ptrace-restricted environments; use release if so.
- Release: `just run-release`
- Clean: `just clean`

## CLI
- `-i, --input PATH` — input file path (default: `input.txt`)
- `-h, --help` — usage
- `-v, --version` — binary name and version (from `justfile` metadata)

Example: `./bin/aoc --input data/example.txt`
