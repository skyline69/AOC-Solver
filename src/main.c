#include "args.h"
#include "task.h"
#include "tools.h"

#include <stddef.h>
#include <stdio.h>

#ifndef PROJECT_NAME
#define PROJECT_NAME "aoc"
#endif

#ifndef PROJECT_VERSION
#define PROJECT_VERSION "dev"
#endif

#define ARENA_SIZE (512 * 512)
#define START_POINTER 50
#define DEFAULT_INPUT "input.txt"

static void printVersion(void) {
  printf("%s %s\n", PROJECT_NAME, PROJECT_VERSION);
}

int main(int argc, char **argv) {
  Arena arena = arenaCreate(ARENA_SIZE);
  if (!arena.base) {
    return 1;
  }

  ArgParseResult args = parseArgs(argc, argv, DEFAULT_INPUT);
  if (args.action == ArgShowHelp) {
    printHelp(args.program_name, DEFAULT_INPUT);
    arenaDestroy(&arena);
    return 0;
  } else if (args.action == ArgShowVersion) {
    printVersion();
    arenaDestroy(&arena);
    return 0;
  } else if (args.action == ArgError) {
    fprintf(stderr, "%s\nUse --help to see usage and options.\n",
            args.error ? args.error : "Invalid arguments");
    arenaDestroy(&arena);
    return 1;
  }

  size_t size;
  char *data = readFile(args.input_path, &size, &arena);
  if (!data) {
    fprintf(stderr, "Failed to read input file: %s\n", args.input_path);
    fprintf(stderr, "Use --help to see usage and options.\n");
    arenaDestroy(&arena);
    return 1;
  }

  unsigned char pointerOne = START_POINTER;
  unsigned char pointerTwo = START_POINTER;
  int countOne = 0;
  int countTwo = 0;

  char *p = data;
  char *end = data + size;

  while (p < end) {
    char dirChar = *p++;
    unsigned int amount = 0;

    while (p < end && *p >= '0' && *p <= '9') {
      amount = amount * 10u + (unsigned)(*p - '0');
      p++;
    }

    if (p < end && *p == '\n') {
      p++;
    }

    enum Direction dir = (dirChar == 'R') ? Right : Left;

    day1RotateFastPartOne(&pointerOne, amount, &countOne, dir);
    day1RotateFastPartTwo(&pointerTwo, amount, &countTwo, dir);
  }

  printf("Part 1: %d\n", countOne);
  printf("Part 2: %d\n", countTwo);

  arenaDestroy(&arena);
  return 0;
}
