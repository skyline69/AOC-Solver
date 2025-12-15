#ifndef VALIDATE_H
#define VALIDATE_H

#include <stddef.h>

// Validate the input file format for a given day. Returns 1 if the input
// matches the expected structure, or 0 on failure. When returning 0 an
// error message is written into errBuf (null-terminated).
int validateInputForDay(unsigned int day, const char *data, size_t size,
                        char *errBuf, size_t errBufSize);

#endif
