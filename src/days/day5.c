#include "days/day5.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct {
  unsigned long long start;
  unsigned long long end;
} Range;

static int cmpRange(const void *a, const void *b) {
  const Range *ra = (const Range *)a;
  const Range *rb = (const Range *)b;
  if (ra->start < rb->start) {
    return -1;
  } else if (ra->start > rb->start) {
    return 1;
  } else if (ra->end < rb->end) {
    return -1;
  } else if (ra->end > rb->end) {
    return 1;
  }
  return 0;
}

static size_t parseRanges(const char *data, Range **out) {
  size_t capacity = 16;
  size_t count = 0;
  Range *ranges = malloc(capacity * sizeof(Range));
  if (!ranges) {
    return 0;
  }

  const char *p = data;
  while (*p) {
    if (*p == '\n') {
      if (*(p + 1) == '\n') {
        p += 2;
        break;
      }
      p++;
      continue;
    }
    char *endPtr;
    unsigned long long start = strtoull(p, &endPtr, 10);
    if (endPtr == p || *endPtr != '-') {
      break;
    }
    p = endPtr + 1;
    unsigned long long end = strtoull(p, &endPtr, 10);
    if (endPtr == p) {
      break;
    }
    if (count == capacity) {
      capacity *= 2;
      Range *newRanges = realloc(ranges, capacity * sizeof(Range));
      if (!newRanges) {
        free(ranges);
        return 0;
      }
      ranges = newRanges;
    }
    ranges[count++] = (Range){start, end};
    while (*endPtr == ' ' || *endPtr == '\t') {
      endPtr++;
    }
    if (*endPtr == '\n') {
      endPtr++;
    }
    p = endPtr;
  }

  qsort(ranges, count, sizeof(Range), cmpRange);

  // Merge overlapping
  size_t write = 0;
  for (size_t i = 0; i < count; ++i) {
    if (write == 0) {
      ranges[write++] = ranges[i];
    } else {
      if (ranges[i].start <= ranges[write - 1].end + 1) {
        if (ranges[i].end > ranges[write - 1].end) {
          ranges[write - 1].end = ranges[i].end;
        }
      } else {
        ranges[write++] = ranges[i];
      }
    }
  }

  *out = ranges;
  return write;
}

static int inRanges(const Range *ranges, size_t count,
                    unsigned long long value) {
  size_t lo = 0;
  size_t hi = count;
  while (lo < hi) {
    size_t mid = lo + (hi - lo) / 2;
    if (value < ranges[mid].start) {
      hi = mid;
    } else if (value > ranges[mid].end) {
      lo = mid + 1;
    } else {
      return 1;
    }
  }
  return 0;
}

static Range *getMergedRanges(const char *data, size_t *outCount) {
  Range *ranges = NULL;
  size_t rangeCount = parseRanges(data, &ranges);
  if (!ranges) {
    return NULL;
  }
  *outCount = rangeCount;
  return ranges;
}

unsigned long long day5Solve(const char *data) {
  size_t rangeCount = 0;
  Range *ranges = getMergedRanges(data, &rangeCount);
  if (!ranges) {
    return 0;
  }

  const char *p = data;
  while (*p && !(*p == '\n' && *(p + 1) == '\n')) {
    p++;
  }
  if (*p == '\n' && *(p + 1) == '\n') {
    p += 2;
  }

  unsigned long long fresh = 0;
  while (*p) {
    while (*p == '\n') {
      p++;
    }
    if (*p == '\0') {
      break;
    }
    char *endPtr;
    unsigned long long id = strtoull(p, &endPtr, 10);
    if (endPtr == p) {
      break;
    }
    if (inRanges(ranges, rangeCount, id)) {
      fresh++;
    }
    p = endPtr;
    while (*p == ' ' || *p == '\t') {
      p++;
    }
    while (*p == ' ' || *p == '\t') {
      p++;
    }
    if (*p == '\n') {
      p++;
    }
  }

  free(ranges);
  return fresh;
}

unsigned long long day5SolvePartTwo(const char *data) {
  size_t rangeCount = 0;
  Range *ranges = getMergedRanges(data, &rangeCount);
  if (!ranges) {
    return 0;
  }

  unsigned long long total = 0;
  for (size_t i = 0; i < rangeCount; ++i) {
    if (ranges[i].end >= ranges[i].start) {
      total += ranges[i].end - ranges[i].start + 1ULL;
    }
  }

  free(ranges);
  return total;
}
