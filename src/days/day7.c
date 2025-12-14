#include "days/day7.h"

#include <stddef.h>
#include <stdlib.h>

unsigned long long day7Solve(const char *data) {
  // Parse grid, find S
  size_t lines = 0;
  size_t width = 0;
  const char *p = data;
  while (*p) {
    size_t len = 0;
    while (p[len] && p[len] != '\n') {
      len++;
    }
    if (len > width) {
      width = len;
    }
    lines++;
    p += len;
    if (*p == '\n') {
      p++;
    }
  }
  if (lines == 0 || width == 0) {
    return 0;
  }

  char **grid = malloc(lines * sizeof(char *));
  if (!grid) {
    return 0;
  }
  p = data;
  size_t sRow = (size_t)-1;
  size_t sCol = (size_t)-1;
  for (size_t r = 0; r < lines; ++r) {
    grid[r] = malloc(width);
    if (!grid[r]) {
      for (size_t k = 0; k < r; ++k) {
        free(grid[k]);
      }
      free(grid);
      return 0;
    }
    size_t len = 0;
    while (p[len] && p[len] != '\n') {
      grid[r][len] = p[len];
      if (p[len] == 'S') {
        sRow = r;
        sCol = len;
      }
      len++;
    }
    for (size_t c = len; c < width; ++c) {
      grid[r][c] = '.';
    }
    p += len;
    if (*p == '\n') {
      p++;
    }
  }

  if (sRow == (size_t)-1) {
    for (size_t r = 0; r < lines; ++r) {
      free(grid[r]);
    }
    free(grid);
    return 0;
  }

  unsigned char *curr = calloc(width, 1);
  unsigned char *next = calloc(width, 1);
  if (!curr || !next) {
    free(curr);
    free(next);
    for (size_t r = 0; r < lines; ++r) {
      free(grid[r]);
    }
    free(grid);
    return 0;
  }

  unsigned long long splits = 0;
  curr[sCol] = 1;

  for (size_t r = sRow + 1; r < lines; ++r) {
    for (size_t c = 0; c < width; ++c) {
      if (!curr[c]) {
        continue;
      }
      char cell = grid[r][c];
      if (cell == '^') {
        splits++;
        if (c > 0) {
          next[c - 1] = 1;
        }
        if (c + 1 < width) {
          next[c + 1] = 1;
        }
      } else {
        next[c] = 1;
      }
    }
    // deduplicate done by bitset arrays
    for (size_t c = 0; c < width; ++c) {
      curr[c] = next[c];
      next[c] = 0;
    }
  }

  free(curr);
  free(next);
  for (size_t r = 0; r < lines; ++r) {
    free(grid[r]);
  }
  free(grid);

  return splits;
}

unsigned long long day7SolvePartTwo(const char *data) {
  // Parse grid, find S
  size_t lines = 0;
  size_t width = 0;
  const char *p = data;
  while (*p) {
    size_t len = 0;
    while (p[len] && p[len] != '\n') {
      len++;
    }
    if (len > width) {
      width = len;
    }
    lines++;
    p += len;
    if (*p == '\n') {
      p++;
    }
  }
  if (lines == 0 || width == 0) {
    return 0;
  }

  char **grid = malloc(lines * sizeof(char *));
  if (!grid) {
    return 0;
  }
  p = data;
  size_t sRow = (size_t)-1;
  size_t sCol = (size_t)-1;
  for (size_t r = 0; r < lines; ++r) {
    grid[r] = malloc(width);
    if (!grid[r]) {
      for (size_t k = 0; k < r; ++k) {
        free(grid[k]);
      }
      free(grid);
      return 0;
    }
    size_t len = 0;
    while (p[len] && p[len] != '\n') {
      grid[r][len] = p[len];
      if (p[len] == 'S') {
        sRow = r;
        sCol = len;
      }
      len++;
    }
    for (size_t c = len; c < width; ++c) {
      grid[r][c] = '.';
    }
    p += len;
    if (*p == '\n') {
      p++;
    }
  }

  if (sRow == (size_t)-1) {
    for (size_t r = 0; r < lines; ++r) {
      free(grid[r]);
    }
    free(grid);
    return 0;
  }

  unsigned long long *curr = calloc(width, sizeof(unsigned long long));
  unsigned long long *next = calloc(width, sizeof(unsigned long long));
  if (!curr || !next) {
    free(curr);
    free(next);
    for (size_t r = 0; r < lines; ++r) {
      free(grid[r]);
    }
    free(grid);
    return 0;
  }

  curr[sCol] = 1ULL;

  for (size_t r = sRow + 1; r < lines; ++r) {
    for (size_t c = 0; c < width; ++c) {
      unsigned long long ways = curr[c];
      if (ways == 0) {
        continue;
      }
      char cell = grid[r][c];
      if (cell == '^') {
        if (c > 0) {
          next[c - 1] += ways;
        }
        if (c + 1 < width) {
          next[c + 1] += ways;
        }
      } else {
        next[c] += ways;
      }
    }
    for (size_t c = 0; c < width; ++c) {
      curr[c] = next[c];
      next[c] = 0;
    }
  }

  unsigned long long total = 0;
  for (size_t c = 0; c < width; ++c) {
    total += curr[c];
  }

  free(curr);
  free(next);
  for (size_t r = 0; r < lines; ++r) {
    free(grid[r]);
  }
  free(grid);

  return total;
}
