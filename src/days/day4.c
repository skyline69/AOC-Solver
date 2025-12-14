#include "days/day4.h"

#include <stdlib.h>
#include <string.h>

unsigned long long day4Solve(const char *data) {
  // First pass: count lines
  size_t lineCount = 0;
  const char *p = data;
  while (*p) {
    lineCount++;
    while (*p && *p != '\n') {
      p++;
    }
    if (*p == '\n') {
      p++;
    }
  }

  if (lineCount == 0) {
    return 0;
  }

  const char **rows = malloc(lineCount * sizeof(const char *));
  size_t *lens = malloc(lineCount * sizeof(size_t));
  if (!rows || !lens) {
    free(rows);
    free(lens);
    return 0;
  }

  p = data;
  for (size_t i = 0; i < lineCount; ++i) {
    rows[i] = p;
    size_t len = 0;
    while (p[len] && p[len] != '\n') {
      len++;
    }
    lens[i] = len;
    p += len;
    if (*p == '\n') {
      p++;
    }
  }

  unsigned long long accessible = 0;

  for (size_t y = 0; y < lineCount; ++y) {
    const char *row = rows[y];
    size_t width = lens[y];
    for (size_t x = 0; x < width; ++x) {
      if (row[x] != '@') {
        continue;
      }
      int neighbors = 0;
      for (int dy = -1; dy <= 1; ++dy) {
        size_t ny = (size_t)((int)y + dy);
        if (dy == 0 && ny == y) {
          // continue; handled below
          ;
        }
        if (dy == 0) {
          // handled in inner loop
        }
      }
      for (int dy = -1; dy <= 1; ++dy) {
        int iy = (int)y + dy;
        if (iy < 0 || iy >= (int)lineCount) {
          continue;
        }
        const char *nrow = rows[iy];
        size_t nwidth = lens[iy];
        for (int dx = -1; dx <= 1; ++dx) {
          if (dx == 0 && dy == 0) {
            continue;
          }
          int ix = (int)x + dx;
          if (ix < 0 || ix >= (int)nwidth) {
            continue;
          }
          if (nrow[ix] == '@') {
            neighbors++;
          }
        }
      }
      if (neighbors < 4) {
        accessible++;
      }
    }
  }

  free(rows);
  free(lens);

  return accessible;
}

typedef struct {
  int x;
  int y;
} Point;

unsigned long long day4SolvePartTwo(const char *data) {
  // Parse grid into mutable buffer
  size_t lineCount = 0;
  const char *p = data;
  size_t width = 0;
  while (*p) {
    size_t len = 0;
    while (p[len] && p[len] != '\n') {
      len++;
    }
    if (len > width) {
      width = len;
    }
    lineCount++;
    p += len;
    if (*p == '\n') {
      p++;
    }
  }
  if (lineCount == 0 || width == 0) {
    return 0;
  }

  size_t gridSize = lineCount * width;
  char *grid = malloc(gridSize);
  unsigned char *adj = malloc(gridSize);
  if (!grid || !adj) {
    free(grid);
    free(adj);
    return 0;
  }
  memset(grid, '.', gridSize);
  memset(adj, 0, gridSize);

  // Fill grid
  p = data;
  for (size_t y = 0; y < lineCount; ++y) {
    size_t len = 0;
    while (p[len] && p[len] != '\n') {
      grid[y * width + len] = p[len];
      len++;
    }
    p += len;
    if (*p == '\n') {
      p++;
    }
  }

  // Precompute adjacency counts
  for (size_t y = 0; y < lineCount; ++y) {
    for (size_t x = 0; x < width; ++x) {
      size_t idx = y * width + x;
      if (grid[idx] != '@') {
        continue;
      }
      for (int dy = -1; dy <= 1; ++dy) {
        int ny = (int)y + dy;
        if (ny < 0 || ny >= (int)lineCount) {
          continue;
        }
        for (int dx = -1; dx <= 1; ++dx) {
          int nx = (int)x + dx;
          if (nx < 0 || nx >= (int)width) {
            continue;
          }
          if (dx == 0 && dy == 0) {
            continue;
          }
          size_t nidx = (size_t)ny * width + (size_t)nx;
          if (grid[nidx] == '@') {
            adj[idx]++;
          }
        }
      }
    }
  }

  // Queue for BFS removals
  size_t capacity = gridSize;
  Point *queue = malloc(capacity * sizeof(Point));
  if (!queue) {
    free(grid);
    free(adj);
    return 0;
  }
  size_t qhead = 0, qtail = 0;

  // Seed queue with accessible rolls
  for (size_t y = 0; y < lineCount; ++y) {
    for (size_t x = 0; x < width; ++x) {
      size_t idx = y * width + x;
      if (grid[idx] == '@' && adj[idx] < 4) {
        queue[qtail++] = (Point){(int)x, (int)y};
      }
    }
  }

  unsigned long long removed = 0;

  while (qhead < qtail) {
    Point pt = queue[qhead++];
    size_t idx = (size_t)pt.y * width + (size_t)pt.x;
    if (grid[idx] != '@') {
      continue;
    }
    if (adj[idx] >= 4) {
      continue;
    }
    grid[idx] = '.'; // remove
    removed++;

    // Update neighbors and enqueue newly accessible ones
    for (int dy = -1; dy <= 1; ++dy) {
      int ny = pt.y + dy;
      if (ny < 0 || ny >= (int)lineCount) {
        continue;
      }
      for (int dx = -1; dx <= 1; ++dx) {
        int nx = pt.x + dx;
        if (nx < 0 || nx >= (int)width) {
          continue;
        }
        if (dx == 0 && dy == 0) {
          continue;
        }
        size_t nidx = (size_t)ny * width + (size_t)nx;
        if (grid[nidx] != '@') {
          continue;
        }
        if (adj[nidx] > 0) {
          adj[nidx]--;
        }
        if (adj[nidx] < 4) {
          queue[qtail++] = (Point){nx, ny};
        }
      }
    }
  }

  free(queue);
  free(grid);
  free(adj);

  return removed;
}
