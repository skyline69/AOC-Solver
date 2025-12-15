.DEFAULT_GOAL := run

PROJECT_NAME := aoc
PROJECT_VERSION := 0.1.0

BIN_DIR := bin
DEBUG_BIN := $(BIN_DIR)/$(PROJECT_NAME)_debug
RELEASE_BIN := $(BIN_DIR)/$(PROJECT_NAME)

SOURCES := src/main.c src/tools.c src/color.c src/cli.c src/validate.c src/days/day1.c src/days/day2.c src/days/day3.c src/days/day4.c src/days/day5.c src/days/day6.c src/days/day7.c src/days/day8.c src/days/day9.c src/days/day10.c src/days/day11.c src/days/day12.c
INCLUDE_FLAGS := -Iinclude
CC := clang

COMMON_FLAGS := -std=c11 -Wall -Wextra -Wconversion -Wshadow -pedantic
METADATA_FLAGS := -DPROJECT_NAME=\"$(PROJECT_NAME)\" -DPROJECT_VERSION=\"$(PROJECT_VERSION)\"
DEBUG_FLAGS := -O2 -g -fsanitize=address -fno-omit-frame-pointer
RELEASE_FLAGS := -O3 -march=native -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables

DEBUG_CFLAGS := $(COMMON_FLAGS) $(METADATA_FLAGS) $(DEBUG_FLAGS)
RELEASE_CFLAGS := $(COMMON_FLAGS) $(METADATA_FLAGS) $(RELEASE_FLAGS)

.PHONY: compile build release run run-release test clean help b r rr c

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(DEBUG_BIN): $(SOURCES) | $(BIN_DIR)
	$(CC) $(INCLUDE_FLAGS) $(DEBUG_CFLAGS) $(SOURCES) -o $@

$(RELEASE_BIN): $(SOURCES) | $(BIN_DIR)
	$(CC) $(INCLUDE_FLAGS) $(RELEASE_CFLAGS) $(SOURCES) -o $@

compile: $(DEBUG_BIN)

build: compile

release: $(RELEASE_BIN)

run: build
	$(DEBUG_BIN)

run-release: release
	$(RELEASE_BIN)

test: release
	./tests/run_samples.sh

clean:
	rm -rf $(BIN_DIR)

help:
	@printf "Targets:\n"
	@printf "  build (b)         Build debug binary -> %s\n" "$(DEBUG_BIN)"
	@printf "  release           Build release binary -> %s\n" "$(RELEASE_BIN)"
	@printf "  run (r)           Build debug binary and run it\n"
	@printf "  run-release (rr)  Build release binary and run it\n"
	@printf "  test              Build release binary and run sample tests\n"
	@printf "  clean (c)         Remove build artifacts\n"
