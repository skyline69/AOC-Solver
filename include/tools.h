#ifndef TOOLS_H
#define TOOLS_H

#include <stddef.h>
#include <stdlib.h>

typedef struct {
  unsigned char *base;
  size_t capacity;
  size_t offset;
} Arena;

Arena arenaCreate(size_t capacity);

void *arenaAlloc(Arena *arena, size_t size);

void arenaReset(Arena *arena);

void arenaDestroy(Arena *arena);

char *readFile(const char *fileName, size_t *outSize, Arena *arena);

char **getLines(char *fileContent, size_t *outCount, Arena *arena);

#endif
