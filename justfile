# Project metadata
project_name := "aoc"
project_version := "0.1.0"

# Paths and build inputs
bin_dir := "bin"
debug_bin := bin_dir + "/" + project_name + "_debug"
release_bin := bin_dir + "/" + project_name
sources := "src/main.c src/task.c src/tools.c src/args.c"
include_flags := "-Iinclude"
cc := "clang"

# Common compiler flags (fast and safe defaults)
common_flags := "-std=c11 -Wall -Wextra -Wconversion -Wshadow -pedantic"
metadata_flags := "-DPROJECT_NAME=\\\"" + project_name + "\\\" -DPROJECT_VERSION=\\\"" + project_version + "\\\""

# Default target
default: run

# Sanitized build (debug / safety)
build:
	mkdir -p {{bin_dir}}
	{{cc}} {{include_flags}} {{common_flags}} {{metadata_flags}} {{sources}} \
		-O2 \
		-g \
		-fsanitize=address \
		-fno-omit-frame-pointer \
		-o {{debug_bin}}

run: build
	./{{debug_bin}}

# Optimized release build with embedded metadata
release:
	mkdir -p {{bin_dir}}
	{{cc}} {{include_flags}} {{common_flags}} {{metadata_flags}} {{sources}} \
		-O3 \
		-march=native \
		-fno-exceptions \
		-fno-unwind-tables \
		-fno-asynchronous-unwind-tables \
		-o {{release_bin}}

run-release: release
	./{{release_bin}}

# Cleanup
clean:
	rm -rf {{bin_dir}}
