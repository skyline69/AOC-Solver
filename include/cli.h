#ifndef CLI_H
#define CLI_H

enum ArgAction { ArgRun, ArgShowHelp, ArgShowAbout, ArgError };
enum PartChoice { PartAll = 0, PartOne = 1, PartTwo = 2 };

typedef struct {
  enum ArgAction action;
  unsigned char day;
  enum PartChoice part;
  const char *input_path;
  const char *program_name;
  const char *error;
} ArgParseResult;

ArgParseResult parseArgs(int argc, char **argv, const char *defaultInput);
void printHelp(const char *programName, const char *defaultInput);

#endif
