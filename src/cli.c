#include "cli.h"
#include "color.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *baseName(const char *path) {
  const char *slash = strrchr(path, '/');
  return slash ? slash + 1 : path;
}

void printHelp(const char *programName, const char *defaultInput) {
  const Palette *p = paletteGet();
  static const char *tree[] = {
      "     *",     "    /.\\",   "   /..'\\",   "   /'.'\\",
      "  /.''.'\\", "  /.'.'.\\", " /'.''.'.\\", " ^^^[_]^^^",
  };

  char header[128];
  char usage[128];
  char options[64];
  char optDay[160];
  char optPart[160];
  char optInput[160];
  char optHelp[96];
  char optVersion[96];

  snprintf(header, sizeof(header), "%s*%s %sAOC 2025 Solver%s %s*%s",
           p->secondary, p->reset, p->primary, p->reset, p->secondary,
           p->reset);
  snprintf(usage, sizeof(usage), "%sUsage:%s %s --day N [--input PATH]",
           p->bold, p->reset, programName);
  snprintf(options, sizeof(options), "%sOptions:%s", p->bold, p->reset);
  snprintf(optDay, sizeof(optDay),
           "  %s-d%s, %s--day N%s       %sRequired:%s day number to run",
           p->secondary, p->reset, p->secondary, p->reset, p->primary,
           p->reset);
  snprintf(optPart, sizeof(optPart),
           "  %s-p%s, %s--part%s        Run part {1|2|all} (default: all)",
           p->accent, p->reset, p->accent, p->reset);
  snprintf(optInput, sizeof(optInput),
           "  %s-i%s, %s--input PATH%s  Input file (default: %s)", p->accent,
           p->reset, p->accent, p->reset, defaultInput);
  snprintf(optHelp, sizeof(optHelp),
           "  %s-h%s, %s--help%s        Show this help message", p->warn,
           p->reset, p->warn, p->reset);
  snprintf(optVersion, sizeof(optVersion),
           "  %s-a%s, %s--about%s       Show about information", p->warn,
           p->reset, p->warn, p->reset);
  const char *info[] = {header,  usage,  options, optDay,
                        optPart, optInput, optHelp, optVersion};

  const size_t treeCount = sizeof(tree) / sizeof(tree[0]);
  const size_t infoCount = sizeof(info) / sizeof(info[0]);
  const size_t lines = (treeCount > infoCount) ? treeCount : infoCount;
  size_t treeWidth = 0;
  for (size_t i = 0; i < treeCount; ++i) {
    size_t len = strlen(tree[i]);
    if (len > treeWidth) {
      treeWidth = len;
    }
  }

  for (size_t i = 0; i < lines; ++i) {
    const char *treeLine = (i < treeCount) ? tree[i] : "";
    const char *infoLine = (i < infoCount) ? info[i] : "";
    size_t printed = 0;
    for (const char *c = treeLine; *c; ++c) {
      if (*c == '*') {
        printf("%s*%s", p->accent, p->reset);
      } else if (*c == ' ') {
        putchar(' ');
      } else {
        printf("%s%c%s", p->primary, *c, p->reset);
      }
      printed++;
    }
    while (printed < treeWidth) {
      putchar(' ');
      printed++;
    }
    if (infoLine[0] != '\0') {
      printf(" %s\n", infoLine);
    } else {
      printf("\n");
    }
  }
}

ArgParseResult parseArgs(int argc, char **argv, const char *defaultInput) {
  static char errorBuf[128];

  ArgParseResult result = {
      .action = ArgRun,
      .day = 0,
      .part = PartAll,
      .input_path = defaultInput,
      .program_name = baseName(argv[0]),
      .error = NULL,
  };

  for (int i = 1; i < argc; ++i) {
    const char *arg = argv[i];

    if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0) {
      result.action = ArgShowHelp;
      return result;
    }

    if (strcmp(arg, "--about") == 0 || strcmp(arg, "-a") == 0) {
      result.action = ArgShowAbout;
      return result;
    }

    if (strcmp(arg, "--input") == 0 || strcmp(arg, "-i") == 0) {
      if (i + 1 >= argc) {
        snprintf(errorBuf, sizeof(errorBuf), "Missing path after %s", arg);
        result.action = ArgError;
        result.error = errorBuf;
        return result;
      }
      result.input_path = argv[++i];
      continue;
    }

    if (strcmp(arg, "--day") == 0 || strcmp(arg, "-d") == 0) {
      if (i + 1 >= argc) {
        snprintf(errorBuf, sizeof(errorBuf), "Missing day after %s", arg);
        result.action = ArgError;
        result.error = errorBuf;
        return result;
      }
      const char *value = argv[++i];
      char *end;
      unsigned long parsed = strtoul(value, &end, 10);
      if (end == value || *end != '\0' || parsed == 0 || parsed > 12UL) {
        snprintf(errorBuf, sizeof(errorBuf), "Invalid day: %s", value);
        result.action = ArgError;
        result.error = errorBuf;
        return result;
      }
      result.day = (unsigned char)parsed;
      continue;
    }

    if (strcmp(arg, "--part") == 0 || strcmp(arg, "-p") == 0) {
      if (i + 1 >= argc) {
        snprintf(errorBuf, sizeof(errorBuf), "Missing part after %s", arg);
        result.action = ArgError;
        result.error = errorBuf;
        return result;
      }
      const char *value = argv[++i];
      if (strcmp(value, "1") == 0) {
        result.part = PartOne;
      } else if (strcmp(value, "2") == 0) {
        result.part = PartTwo;
      } else if (strcmp(value, "all") == 0 || strcmp(value, "both") == 0) {
        result.part = PartAll;
      } else {
        snprintf(errorBuf, sizeof(errorBuf), "Invalid part: %s", value);
        result.action = ArgError;
        result.error = errorBuf;
        return result;
      }
      continue;
    }

    snprintf(errorBuf, sizeof(errorBuf), "Unknown option: %s", arg);
    result.action = ArgError;
    result.error = errorBuf;
    return result;
  }

  if (result.action == ArgRun && result.day == 0) {
    snprintf(errorBuf, sizeof(errorBuf), "Missing required option: --day");
    result.action = ArgError;
    result.error = errorBuf;
    return result;
  }

  return result;
}
