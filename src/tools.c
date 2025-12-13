#include "tools.h"

#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALIGN_UP(x, a) (((x) + (a)-1) & ~((a)-1))

Arena arenaCreate(size_t capacity) {
  Arena arena;
  arena.base = malloc(capacity);
  arena.capacity = capacity;
  arena.offset = 0;
  return arena;
}

void *arenaAlloc(Arena *arena, size_t size) {
  size = ALIGN_UP(size, alignof(max_align_t));
  if (arena->offset + size > arena->capacity) {
    return NULL;
  }
  void *ptr = arena->base + arena->offset;
  arena->offset += size;
  return ptr;
}

void arenaReset(Arena *arena) { arena->offset = 0; }

void arenaDestroy(Arena *arena) { free(arena->base); }

char *readFile(const char *fileName, size_t *outSize, Arena *arena) {
  FILE *f = fopen(fileName, "rb");
  if (!f) {
    return NULL;
  }

  if (fseek(f, 0, SEEK_END) != 0) {
    fclose(f);
    return NULL;
  }

  long size = ftell(f);
  if (size < 0) {
    fclose(f);
    return NULL;
  }

  rewind(f);

  char *buf = arenaAlloc(arena, (size_t)size + 1);
  if (!buf) {
    fclose(f);
    return NULL;
  }

  if (fread(buf, 1, (size_t)size, f) != (size_t)size) {
    fclose(f);
    return NULL;
  }

  fclose(f);

  buf[size] = '\0';
  *outSize = (size_t)size;
  return buf;
}

char **getLines(char *fileContent, size_t *outCount, Arena *arena) {
  size_t len = strlen(fileContent);
  size_t lines = 0;

  for (size_t i = 0; i < len; ++i) {
    if (fileContent[i] == '\n') {
      lines++;
    }
  }

  if (len > 0 && fileContent[len - 1] != '\n') {
    lines++;
  }

  char **result = arenaAlloc(arena, lines * sizeof(char *));
  if (!result) {
    return NULL;
  }

  size_t idx = 0;
  char *start = fileContent;

  for (size_t i = 0; i < len; ++i) {
    if (fileContent[i] == '\n') {
      fileContent[i] = '\0';
      result[idx++] = start;
      start = fileContent + i + 1;
    }
  }

  if (start < fileContent + len) {
    result[idx++] = start;
  }

  *outCount = idx;
  return result;
}
