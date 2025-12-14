#include "day3.h"

#include <stddef.h>

unsigned long long day3Solve(const char *data) {
  unsigned long long sum = 0;
  const char *p = data;

  while (*p) {
    int bestLead = -1;
    int best = -1;

    while (*p && *p != '\n') {
      char c = *p++;
      if (c < '0' || c > '9') {
        continue;
      }
      int digit = c - '0';
      if (bestLead >= 0) {
        int candidate = bestLead * 10 + digit;
        if (candidate > best) {
          best = candidate;
        }
      }
      if (digit > bestLead) {
        bestLead = digit;
      }
    }

    if (best >= 0) {
      sum += (unsigned long long)best;
    }

    if (*p == '\n') {
      p++;
    }
  }

  return sum;
}

static unsigned long long maxSubsequenceK(const char *line, int k) {
  const char *p = line;
  int total = 0;
  while (*p && *p != '\n') {
    if (*p >= '0' && *p <= '9') {
      total++;
    }
    p++;
  }

  char chosen[12];
  int len = 0;
  p = line;
  int remaining = total;

  while (*p && *p != '\n') {
    char c = *p++;
    if (c < '0' || c > '9') {
      continue;
    }
    remaining--;
    while (len > 0 && chosen[len - 1] < c && (len + remaining) >= k) {
      len--;
    }
    if (len < k) {
      chosen[len++] = c;
    }
  }

  unsigned long long value = 0;
  for (int i = 0; i < len; ++i) {
    value = value * 10ULL + (unsigned long long)(chosen[i] - '0');
  }
  return value;
}

unsigned long long day3SolvePartTwo(const char *data) {
  unsigned long long sum = 0;
  const char *p = data;

  while (*p) {
    unsigned long long value = maxSubsequenceK(p, 12);
    sum += value;

    while (*p && *p != '\n') {
      p++;
    }
    if (*p == '\n') {
      p++;
    }
  }

  return sum;
}
