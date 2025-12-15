# Global settings
set shell := ["bash", "-eu", "-o", "pipefail", "-c"]
set quiet

# Project metadata
project_name := "aoc"
project_version := "0.1.0"

# Paths
root := justfile_directory()
bin_dir := root + "/bin"
debug_bin := bin_dir + "/" + project_name + "_debug"
release_bin := bin_dir + "/" + project_name

# Inputs
sources := "src/main.c src/tools.c src/color.c src/cli.c src/validate.c src/days/day1.c src/days/day2.c src/days/day3.c src/days/day4.c src/days/day5.c src/days/day6.c src/days/day7.c src/days/day8.c src/days/day9.c src/days/day10.c src/days/day11.c src/days/day12.c"
include_flags := "-Iinclude"
cc := "clang"

# Flags
common_flags := "-std=c11 -Wall -Wextra -Wconversion -Wshadow -pedantic"
metadata_flags := "-DPROJECT_NAME=\\\"" + project_name + "\\\" -DPROJECT_VERSION=\\\"" + project_version + "\\\""

debug_flags := "-O2 -g -fsanitize=address -fno-omit-frame-pointer"
release_flags := "-O3 -march=native -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables"

# Aliases
alias b := build
alias r := run
alias rr := run-release
alias c := clean

# Build pipeline
[group("build")]
compile profile="debug":
  mkdir -p {{bin_dir}}

  {{cc}} {{include_flags}} {{common_flags}} {{metadata_flags}} {{sources}} \
    {{ if profile == "release" { release_flags } else { debug_flags } }} \
    -o {{ if profile == "release" { release_bin } else { debug_bin } }}

[group("build")]
build profile="debug": (compile profile)

# Run targets
[group("run")]
[default]
run profile="debug": (build profile)
  {{ if profile == "release" { release_bin } else { debug_bin } }}

[group("build")]
release: (build "release")

[group("run")]
run-release: (run "release")

[group("test")]
test: (release)
  ./tests/run_samples.sh

# Maintenance
[group("maintenance")]
clean:
  rm -rf {{bin_dir}}

[group("meta")]
help:
  @just --list
