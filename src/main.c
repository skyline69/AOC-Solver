#include "cli.h"
#include "color.h"
#include "days/day1.h"
#include "days/day2.h"
#include "days/day3.h"
#include "days/day4.h"
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
  const Palette *p = paletteGet();
  printf("%s%s%s %s%s%s\n", p->primary, PROJECT_NAME, p->reset, p->secondary,
         PROJECT_VERSION, p->reset);
}

static int runDay1(const char *data, size_t size, enum PartChoice part) {
  const Palette *colors = paletteGet();
  unsigned char pointerOne = START_POINTER;
  unsigned char pointerTwo = START_POINTER;
  int countOne = 0;
  int countTwo = 0;

  const char *pData = data;
  const char *end = data + size;

  while (pData < end) {
    char dirChar = *pData++;
    unsigned int amount = 0;

    while (pData < end && *pData >= '0' && *pData <= '9') {
      amount = amount * 10u + (unsigned)(*pData - '0');
      pData++;
    }

    if (pData < end && *pData == '\n') {
      pData++;
    }

    enum Direction dir = (dirChar == 'R') ? Right : Left;

    day1RotateFastPartOne(&pointerOne, amount, &countOne, dir);
    day1RotateFastPartTwo(&pointerTwo, amount, &countTwo, dir);
  }

  if (part == PartAll || part == PartOne) {
    printf("%s[Part 1]%s %s%d%s\n", colors->secondary, colors->reset,
           colors->primary, countOne, colors->reset);
  }
  if (part == PartAll || part == PartTwo) {
    printf("%s[Part 2]%s %s%d%s\n", colors->secondary, colors->reset,
           colors->primary, countTwo, colors->reset);
  }

  return 0;
}

static int runDay2(const char *data, enum PartChoice part) {
  const Palette *colors = paletteGet();
  if (part == PartAll || part == PartOne) {
    unsigned long long part1 = day2Solve(data);
    printf("%s[Part 1]%s %s%llu%s\n", colors->secondary, colors->reset,
           colors->primary, part1, colors->reset);
  }
  if (part == PartAll || part == PartTwo) {
    unsigned long long part2 = day2SolvePartTwo(data);
    printf("%s[Part 2]%s %s%llu%s\n", colors->secondary, colors->reset,
           colors->primary, part2, colors->reset);
  }
  return 0;
}

static int runDay3(const char *data, enum PartChoice part) {
  const Palette *colors = paletteGet();
  if (part == PartAll || part == PartOne) {
    unsigned long long part1 = day3Solve(data);
    printf("%s[Part 1]%s %s%llu%s\n", colors->secondary, colors->reset,
           colors->primary, part1, colors->reset);
  }
  if (part == PartAll || part == PartTwo) {
    unsigned long long part2 = day3SolvePartTwo(data);
    printf("%s[Part 2]%s %s%llu%s\n", colors->secondary, colors->reset,
           colors->primary, part2, colors->reset);
  }
  return 0;
}

int main(int argc, char **argv) {
  Arena arena = arenaCreate(ARENA_SIZE);
  if (!arena.base) {
    return 1;
  }

  ArgParseResult args = parseArgs(argc, argv, DEFAULT_INPUT);
  const Palette *p = paletteGet();
  if (args.action == ArgShowHelp) {
    printHelp(args.program_name, DEFAULT_INPUT);
    printf("\n");
    arenaDestroy(&arena);
    return 0;
  } else if (args.action == ArgShowVersion) {
    printVersion();
    arenaDestroy(&arena);
    return 0;
  } else if (args.action == ArgError) {
    fprintf(stderr, "%s%s%s\n%sUse --help to see usage and options.%s\n",
            p->error, args.error ? args.error : "Invalid arguments", p->reset,
            p->warn, p->reset);
    arenaDestroy(&arena);
    return 1;
  }

  size_t size;
  char *data = readFile(args.input_path, &size, &arena);
  if (!data) {
    fprintf(stderr, "%sFailed to read input file:%s %s\n", p->error, p->reset,
            args.input_path);
    fprintf(stderr, "%sUse --help to see usage and options.%s\n", p->warn,
            p->reset);
    arenaDestroy(&arena);
    return 1;
  }

  int exitCode = 0;
  if (args.day == 1) {
    exitCode = runDay1(data, size, args.part);
  } else if (args.day == 2) {
    exitCode = runDay2(data, args.part);
  } else if (args.day == 3) {
    exitCode = runDay3(data, args.part);
  } else if (args.day == 4) {
    if (args.part == PartAll || args.part == PartOne) {
      unsigned long long part1 = day4Solve(data);
      printf("%s[Part 1]%s %s%llu%s\n", p->secondary, p->reset, p->primary,
             part1, p->reset);
    }
    if (args.part == PartAll || args.part == PartTwo) {
      unsigned long long part2 = day4SolvePartTwo(data);
      printf("%s[Part 2]%s %s%llu%s\n", p->secondary, p->reset, p->primary,
             part2, p->reset);
    }
  } else {
    fprintf(stderr, "%sUnsupported day:%s %u\n", p->error, p->reset,
            (unsigned)args.day);
    fprintf(stderr, "%sCurrently supported days: 1, 2, 3, 4%s\n", p->warn,
            p->reset);
    exitCode = 1;
  }

  arenaDestroy(&arena);
  return exitCode;
}
