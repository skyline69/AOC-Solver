# aoc
<img width="650" height="178" alt="help-message" src="https://github.com/user-attachments/assets/16c6bc99-59d7-49ff-88ef-b1806d764c79" />


Small C CLI for solving AOC 2025 quests

## Build & Run
- Debug (ASan): `just run`  
  - Note: AddressSanitizer may not run in ptrace-restricted environments; use release if so.
- Release: `just run-release`
- Clean: `just clean`

## CLI
- `-d, --day N` — required day to run
- `-p, --part {1|2|all}` — choose part(s) to run (default: all)
- `-i, --input PATH` — input file path (default: `input.txt`)
- `-h, --help` — usage
- `-v, --version` — binary name and version (from `justfile` metadata)

Example: `./bin/aoc --day 1 --part all --input data/example.txt`

Currently implemented: Days 1–5.
