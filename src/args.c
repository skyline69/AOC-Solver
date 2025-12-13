#include "args.h"

#include <stdio.h>
#include <string.h>

static const char *baseName(const char *path) {
  const char *slash = strrchr(path, '/');
  return slash ? slash + 1 : path;
}

void printHelp(const char *programName, const char *defaultInput) {
  printf("Usage: %s [--input PATH]\n", programName);
  printf("Options:\n");
  printf("  -i, --input PATH   Path to input file (default: %s)\n", defaultInput);
  printf("  -h, --help         Show this help message\n");
  printf("  -v, --version      Show version information\n");
}

ArgParseResult parseArgs(int argc, char **argv, const char *defaultInput) {
  static char errorBuf[128];

  ArgParseResult result = {
      .action = ArgRun,
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

    if (strcmp(arg, "--version") == 0 || strcmp(arg, "-v") == 0) {
      result.action = ArgShowVersion;
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

    snprintf(errorBuf, sizeof(errorBuf), "Unknown option: %s", arg);
    result.action = ArgError;
    result.error = errorBuf;
    return result;
  }

  return result;
}
