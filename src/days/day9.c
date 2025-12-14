#include "days/day9.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
  int x;
  int y;
} Point2;

typedef struct {
  int a;
  int b;
} Interval;

typedef struct {
  Interval *arr;
  int count;
  int cap;
} RowIntervals;

typedef struct {
  int *arr;
  int count;
  int cap;
} IntList;

static int cmpInterval(const void *a, const void *b) {
  const Interval *ia = (const Interval *)a;
  const Interval *ib = (const Interval *)b;
  if (ia->a != ib->a) return ia->a - ib->a;
  return ia->b - ib->b;
}

static int cmpInt(const void *a, const void *b) {
  int ia = *(const int *)a;
  int ib = *(const int *)b;
  return ia - ib;
}

static void rowAdd(RowIntervals *row, int a, int b) {
  if (a > b) {
    int t = a;
    a = b;
    b = t;
  }
  if (row->count == row->cap) {
    int newCap = row->cap ? row->cap * 2 : 4;
    Interval *n = realloc(row->arr, (size_t)newCap * sizeof(Interval));
    if (!n) return;
    row->arr = n;
    row->cap = newCap;
  }
  row->arr[row->count++] = (Interval){a, b};
}

static void mergeRow(RowIntervals *row) {
  if (row->count == 0) return;
  qsort(row->arr, (size_t)row->count, sizeof(Interval), cmpInterval);
  int w = 0;
  for (int i = 0; i < row->count; ++i) {
    if (w == 0 || row->arr[i].a > row->arr[w - 1].b + 1) {
      row->arr[w++] = row->arr[i];
    } else if (row->arr[i].b > row->arr[w - 1].b) {
      row->arr[w - 1].b = row->arr[i].b;
    }
  }
  row->count = w;
}

static int rowCovered(const RowIntervals *row, int x0, int x1) {
  if (row->count == 0) return 0;
  if (row->count == 1) {
    return row->arr[0].a <= x0 && row->arr[0].b >= x1;
  }
  int l = 0;
  int r = row->count - 1;
  while (l <= r) {
    int m = (l + r) / 2;
    Interval in = row->arr[m];
    if (in.a > x0) {
      r = m - 1;
    } else if (in.b < x0) {
      l = m + 1;
    } else {
      return in.b >= x1;
    }
  }
  return 0;
}

static int rowsEqual(const RowIntervals *a, const RowIntervals *b) {
  if (a->count != b->count) return 0;
  for (int i = 0; i < a->count; ++i) {
    if (a->arr[i].a != b->arr[i].a || a->arr[i].b != b->arr[i].b) return 0;
  }
  return 1;
}

static void intAdd(IntList *list, int v) {
  if (list->count == list->cap) {
    int newCap = list->cap ? list->cap * 2 : 8;
    int *n = realloc(list->arr, (size_t)newCap * sizeof(int));
    if (!n) return;
    list->arr = n;
    list->cap = newCap;
  }
  list->arr[list->count++] = v;
}

static Point2 *parsePoints(const char *data, size_t *outCount) {
  size_t capacity = 64;
  size_t count = 0;
  Point2 *pts = malloc(capacity * sizeof(Point2));
  const char *p = data;
  while (*p) {
    while (*p == '\n' || *p == '\r') p++;
    if (*p == '\0') break;
    char *end;
    int x = (int)strtol(p, &end, 10);
    if (end == p || *end != ',') break;
    p = end + 1;
    int y = (int)strtol(p, &end, 10);
    if (end == p) break;
    if (count == capacity) {
      capacity *= 2;
      Point2 *n = realloc(pts, capacity * sizeof(Point2));
      if (!n) {
        free(pts);
        return NULL;
      }
      pts = n;
    }
    pts[count++] = (Point2){x, y};
    p = end;
    while (*p == ' ' || *p == '\t') p++;
    if (*p == '\n') p++;
  }

  *outCount = count;
  return pts;
}

unsigned long long day9Solve(const char *data) {
  size_t count = 0;
  Point2 *pts = parsePoints(data, &count);
  if (!pts || count < 2) {
    free(pts);
    return 0;
  }
  unsigned long long best = 0;
  for (size_t i = 0; i < count; ++i) {
    for (size_t j = i + 1; j < count; ++j) {
      long dx = (long)pts[i].x - (long)pts[j].x;
      long dy = (long)pts[i].y - (long)pts[j].y;
      if (dx == 0 || dy == 0) continue;
      unsigned long long area =
          ((unsigned long long)(dx < 0 ? -dx : dx) + 1ULL) *
          ((unsigned long long)(dy < 0 ? -dy : dy) + 1ULL);
      if (area > best) best = area;
    }
  }
  free(pts);
  return best;
}

unsigned long long day9SolvePartTwo(const char *data) {
  size_t count = 0;
  Point2 *pts = parsePoints(data, &count);
  if (!pts || count < 2) {
    free(pts);
    return 0;
  }

  int minX = pts[0].x, maxX = pts[0].x, minY = pts[0].y, maxY = pts[0].y;
  for (size_t i = 1; i < count; ++i) {
    if (pts[i].x < minX) minX = pts[i].x;
    if (pts[i].x > maxX) maxX = pts[i].x;
    if (pts[i].y < minY) minY = pts[i].y;
    if (pts[i].y > maxY) maxY = pts[i].y;
  }

  int height = maxY - minY + 1;
  RowIntervals *rows = calloc((size_t)height, sizeof(RowIntervals));
  IntList *cross = calloc((size_t)height, sizeof(IntList));
  if (!rows || !cross) {
    free(rows);
    free(cross);
    free(pts);
    return 0;
  }

  // Boundary: segments between consecutive points (wrap)
  for (size_t i = 0; i < count; ++i) {
    size_t j = (i + 1) % count;
    int x1 = pts[i].x, y1 = pts[i].y;
    int x2 = pts[j].x, y2 = pts[j].y;
    if (y1 == y2) {
      int a = x1 < x2 ? x1 : x2;
      int b = x1 < x2 ? x2 : x1;
      rowAdd(&rows[y1 - minY], a, b);
    } else if (x1 == x2) {
      int a = y1 < y2 ? y1 : y2;
      int b = y1 < y2 ? y2 : y1;
      for (int y = a; y <= b; ++y) {
        rowAdd(&rows[y - minY], x1, x1);
      }
    }
  }

  // Fill interior via scanline intersections of vertical edges
  for (size_t i = 0; i < count; ++i) {
    size_t j = (i + 1) % count;
    int x1 = pts[i].x, y1 = pts[i].y;
    int x2 = pts[j].x, y2 = pts[j].y;
    if (x1 != x2) continue;
    int yStart = y1 < y2 ? y1 : y2;
    int yEnd = y1 < y2 ? y2 : y1;
    for (int y = yStart; y < yEnd; ++y) {
      intAdd(&cross[y - minY], x1);
      rowAdd(&rows[y - minY], x1, x1);
    }
  }

  // Fill interior between intersections
  for (int r = 0; r < height; ++r) {
    IntList *ls = &cross[r];
    if (ls->count >= 2) {
      qsort(ls->arr, (size_t)ls->count, sizeof(int), cmpInt);
      for (int i = 0; i + 1 < ls->count; i += 2) {
        rowAdd(&rows[r], ls->arr[i], ls->arr[i + 1]);
      }
    }
  }

  for (int rr = 0; rr < height; ++rr) {
    mergeRow(&rows[rr]);
  }

  // Compress rows into runs of identical coverage
  typedef struct {
    int y0;
    int y1;
    RowIntervals *row;
  } RowRun;
  RowRun *runs = malloc((size_t)height * sizeof(RowRun));
  int runCount = 0;
  int r = 0;
  while (r < height) {
    int start = r;
    int end = r;
    while (end + 1 < height && rowsEqual(&rows[end + 1], &rows[start])) {
      end++;
    }
    runs[runCount++] = (RowRun){start + minY, end + minY, &rows[start]};
    r = end + 1;
  }

  unsigned long long best = 0;
  for (size_t i = 0; i < count; ++i) {
    for (size_t j = i + 1; j < count; ++j) {
      int x0 = pts[i].x < pts[j].x ? pts[i].x : pts[j].x;
      int x1 = pts[i].x > pts[j].x ? pts[i].x : pts[j].x;
      int y0 = pts[i].y < pts[j].y ? pts[i].y : pts[j].y;
      int y1 = pts[i].y > pts[j].y ? pts[i].y : pts[j].y;
      if (x0 == x1 || y0 == y1) {
        continue;
      }
      unsigned long long area =
          (unsigned long long)(x1 - x0 + 1) * (unsigned long long)(y1 - y0 + 1);
      if (area <= best) {
        continue;
      }
      int ok = 1;
      for (int k = 0; k < runCount; ++k) {
        if (runs[k].y1 < y0 || runs[k].y0 > y1) {
          continue;
        }
        const RowIntervals *row = runs[k].row;
        if (!rowCovered(row, x0, x1)) {
          ok = 0;
          break;
        }
        if (runs[k].y1 >= y1) {
          break;
        }
      }
      if (ok) {
        best = area;
      }
    }
  }

  for (int rr = 0; rr < height; ++rr) {
    free(rows[rr].arr);
    free(cross[rr].arr);
  }
  free(rows);
  free(cross);
  free(runs);
  free(pts);
  return best;
}
