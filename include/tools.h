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

static inline int isDigit(char c) {
  unsigned int v = (unsigned int)(unsigned char)c;
  return v - (unsigned char)'0' <= 9u;
}

unsigned long long parseULL(const char *s, const char **endOut);

static const int NEIGHBOR_DX[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
static const int NEIGHBOR_DY[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

#endif
