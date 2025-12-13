#ifndef ARGS_H
#define ARGS_H

enum ArgAction { ArgRun, ArgShowHelp, ArgShowVersion, ArgError };

typedef struct {
  enum ArgAction action;
  const char *input_path;
  const char *program_name;
  const char *error;
} ArgParseResult;

ArgParseResult parseArgs(int argc, char **argv, const char *defaultInput);
void printHelp(const char *programName, const char *defaultInput);

#endif
