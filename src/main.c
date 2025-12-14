#include "cli.h"
#include "color.h"
#include "days/day1.h"
#include "days/day2.h"
#include "days/day3.h"
#include "days/day4.h"
#include "days/day5.h"
#include "days/day6.h"
#include "days/day7.h"
#include "days/day8.h"
#include "tools.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

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
  static const char *ginger[] = {
      "   ,-.",
      " _(*_*)_",
      "(_  o  _)",
      "  / o \\",
      " (_/ \\_)",
  };

  char ver[128];
  char author[128];
  char support[128];
  char license[160];
  char year[64];

  snprintf(ver, sizeof(ver), "%s%s%s %s%s%s", p->primary, PROJECT_NAME,
           p->reset, p->secondary, PROJECT_VERSION, p->reset);
  snprintf(author, sizeof(author), "%sAuthor:%s %sskyline%s", p->bold,
           p->reset, p->primary, p->reset);
  snprintf(support, sizeof(support), "%sDays:%s %s1–8%s", p->bold, p->reset,
           p->primary, p->reset);
  snprintf(license, sizeof(license), "%sLicense:%s %sMIT%s", p->bold, p->reset,
           p->primary, p->reset);
  snprintf(year, sizeof(year), "%sYear made:%s %s2025%s", p->bold, p->reset,
           p->primary, p->reset);

  const char *info[] = {ver, author, support, license, year};
  const size_t gcount = sizeof(ginger) / sizeof(ginger[0]);
  const size_t icount = sizeof(info) / sizeof(info[0]);
  size_t width = 0;
  for (size_t i = 0; i < gcount; ++i) {
    size_t len = strlen(ginger[i]);
    if (len > width) {
      width = len;
    }
  }

  size_t lines = (gcount > icount) ? gcount : icount;
  for (size_t i = 0; i < lines; ++i) {
    const char *g = (i < gcount) ? ginger[i] : "";
    const char *inf = (i < icount) ? info[i] : "";
    printf("%s%-*s%s %s\n", p->accent, (int)width, g, p->reset, inf);
  }
  printf("\n");
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
  } else if (args.action == ArgShowAbout) {
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
  } else if (args.day == 5) {
    if (args.part == PartAll || args.part == PartOne) {
      unsigned long long part1 = day5Solve(data);
      printf("%s[Part 1]%s %s%llu%s\n", p->secondary, p->reset, p->primary,
             part1, p->reset);
    }
    if (args.part == PartAll || args.part == PartTwo) {
      unsigned long long part2 = day5SolvePartTwo(data);
      printf("%s[Part 2]%s %s%llu%s\n", p->secondary, p->reset, p->primary,
             part2, p->reset);
    }
  } else if (args.day == 6) {
    if (args.part == PartAll || args.part == PartOne) {
      unsigned long long part1 = day6Solve(data);
      printf("%s[Part 1]%s %s%llu%s\n", p->secondary, p->reset, p->primary,
             part1, p->reset);
    }
    if (args.part == PartAll || args.part == PartTwo) {
      unsigned long long part2 = day6SolvePartTwo(data);
      printf("%s[Part 2]%s %s%llu%s\n", p->secondary, p->reset, p->primary,
             part2, p->reset);
    }
  } else if (args.day == 7) {
    if (args.part == PartAll || args.part == PartOne) {
      unsigned long long part1 = day7Solve(data);
      printf("%s[Part 1]%s %s%llu%s\n", p->secondary, p->reset, p->primary,
             part1, p->reset);
    }
    if (args.part == PartAll || args.part == PartTwo) {
      unsigned long long part2 = day7SolvePartTwo(data);
      printf("%s[Part 2]%s %s%llu%s\n", p->secondary, p->reset, p->primary,
             part2, p->reset);
    }
  } else if (args.day == 8) {
    if (args.part == PartAll || args.part == PartOne) {
      unsigned long long part1 = day8Solve(data);
      printf("%s[Part 1]%s %s%llu%s\n", p->secondary, p->reset, p->primary,
             part1, p->reset);
    }
    if (args.part == PartAll || args.part == PartTwo) {
      unsigned long long part2 = day8SolvePartTwo(data);
      printf("%s[Part 2]%s %s%llu%s\n", p->secondary, p->reset, p->primary,
             part2, p->reset);
    }
  } else {
    fprintf(stderr, "%sUnsupported day:%s %u\n", p->error, p->reset,
            (unsigned)args.day);
    fprintf(stderr, "%sCurrently supported days: 1, 2, 3, 4, 5, 6, 7, 8%s\n",
            p->warn, p->reset);
    exitCode = 1;
  }

  arenaDestroy(&arena);
  return exitCode;
}
