#include "days/day6.h"

#include "tools.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char op;
  unsigned long long *nums;
  size_t count;
  size_t capacity;
} Problem;

static void problemAddNum(Problem *p, unsigned long long v) {
  if (p->count == p->capacity) {
    size_t newCap = p->capacity ? p->capacity * 2 : 4;
    unsigned long long *nn =
        realloc(p->nums, newCap * sizeof(unsigned long long));
    if (!nn) {
      return;
    }
    p->nums = nn;
    p->capacity = newCap;
  }
  p->nums[p->count++] = v;
}

static unsigned long long problemEval(const Problem *p) {
  if (p->count == 0) {
    return 0;
  }
  if (p->op == '+') {
    unsigned long long s = 0;
    for (size_t i = 0; i < p->count; ++i) {
      s += p->nums[i];
    }
    return s;
  }
  unsigned long long mul = 1;
  for (size_t i = 0; i < p->count; ++i) {
    mul *= p->nums[i];
  }
  return mul;
}

unsigned long long day6Solve(const char *data) {
  // Count lines and width
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

  // Pad lines to width
  char **grid = malloc(lines * sizeof(char *));
  if (!grid) {
    return 0;
  }
  p = data;
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
      len++;
    }
    for (size_t c = len; c < width; ++c) {
      grid[r][c] = ' ';
    }
    p += len;
    if (*p == '\n') {
      p++;
    }
  }

  // Find last non-empty row (any non-space char)
  long opRow = -1;
  for (long r = (long)lines - 1; r >= 0; --r) {
    int has = 0;
    for (size_t c = 0; c < width; ++c) {
      if (grid[r][c] != ' ') {
        has = 1;
        break;
      }
    }
    if (has) {
      opRow = r;
      break;
    }
  }
  if (opRow < 0) {
    for (size_t r = 0; r < lines; ++r) {
      free(grid[r]);
    }
    free(grid);
    return 0;
  }

  // Identify column groups separated by all-space columns
  size_t col = 0;
  unsigned long long total = 0;
  while (col < width) {
    // Skip empty columns (all spaces)
    size_t start = col;
    int allSpace = 1;
    while (start < width) {
      allSpace = 1;
      for (size_t r = 0; r <= (size_t)opRow; ++r) {
        if (grid[r][start] != ' ') {
          allSpace = 0;
          break;
        }
      }
      if (!allSpace) {
        break;
      }
      start++;
    }
    if (start >= width) {
      break;
    }
    size_t end = start;
    while (end < width) {
      int colEmpty = 1;
      for (size_t r = 0; r <= (size_t)opRow; ++r) {
        if (grid[r][end] != ' ') {
          colEmpty = 0;
          break;
        }
      }
      if (colEmpty) {
        break;
      }
      end++;
    }

    Problem prob = {.op = '+', .nums = NULL, .count = 0, .capacity = 0};

    // Operator at opRow in this column span
    for (size_t c = start; c < end; ++c) {
      char ch = grid[opRow][c];
      if (ch == '+' || ch == '*') {
        prob.op = ch;
        break;
      }
    }

    // Collect numbers column span: for each row above opRow, parse digits at this span
    for (size_t r = 0; r < (size_t)opRow; ++r) {
      const char *row = grid[r];
      size_t c = start;
      // skip leading spaces in this span
      while (c < end && row[c] == ' ') {
        c++;
      }
      if (c >= end || row[c] < '0' || row[c] > '9') {
        continue;
      }
      unsigned long long val = 0;
      while (c < end && row[c] >= '0' && row[c] <= '9') {
        val = val * 10ULL + (unsigned long long)(row[c] - '0');
        c++;
      }
      problemAddNum(&prob, val);
    }

    total += problemEval(&prob);
    free(prob.nums);

    col = end + 1;
  }

  for (size_t r = 0; r < lines; ++r) {
    free(grid[r]);
  }
  free(grid);

  return total;
}

unsigned long long day6SolvePartTwo(const char *data) {
  // Count lines and width
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
      len++;
    }
    for (size_t c = len; c < width; ++c) {
      grid[r][c] = ' ';
    }
    p += len;
    if (*p == '\n') {
      p++;
    }
  }

  long opRow = -1;
  for (long r = (long)lines - 1; r >= 0; --r) {
    int has = 0;
    for (size_t c = 0; c < width; ++c) {
      if (grid[r][c] != ' ') {
        has = 1;
        break;
      }
    }
    if (has) {
      opRow = r;
      break;
    }
  }
  if (opRow < 0) {
    for (size_t r = 0; r < lines; ++r) {
      free(grid[r]);
    }
    free(grid);
    return 0;
  }

  // Process columns right-to-left
  unsigned long long total = 0;
  long col = (long)width - 1;
  while (col >= 0) {
    // Skip empty columns
    long end = col;
    while (end >= 0) {
      int allSpace = 1;
      for (long r = 0; r <= opRow; ++r) {
        if (grid[r][(size_t)end] != ' ') {
          allSpace = 0;
          break;
        }
      }
      if (!allSpace) {
        break;
      }
      end--;
    }
    if (end < 0) {
      break;
    }
    long start = end;
    while (start >= 0) {
      int empty = 1;
      for (long r = 0; r <= opRow; ++r) {
        if (grid[r][(size_t)start] != ' ') {
          empty = 0;
          break;
        }
      }
      if (empty) {
        break;
      }
      start--;
    }
    start++; // move to first non-empty column

    Problem prob = {.op = '+', .nums = NULL, .count = 0, .capacity = 0};
    // Operator from opRow
    for (long c = start; c <= end; ++c) {
      char ch = grid[opRow][(size_t)c];
      if (ch == '+' || ch == '*') {
        prob.op = ch;
        break;
      }
    }

    // Read digits top->bottom to form numbers
    for (long c = start; c <= end; ++c) {
      unsigned long long val = 0;
      int hasDigit = 0;
      for (long r = 0; r < opRow; ++r) {
        char ch = grid[(size_t)r][(size_t)c];
        if (ch >= '0' && ch <= '9') {
          val = val * 10ULL + (unsigned long long)(ch - '0');
          hasDigit = 1;
        }
      }
      if (hasDigit) {
        problemAddNum(&prob, val);
      }
    }

    total += problemEval(&prob);
    free(prob.nums);

    col = start - 2; // move left past spacer
  }

  for (size_t r = 0; r < lines; ++r) {
    free(grid[r]);
  }
  free(grid);

  return total;
}
