#include "days/day8.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
  int x;
  int y;
  int z;
} Point3;

typedef struct {
  unsigned long long dist;
  int a;
  int b;
} Edge;

typedef struct {
  Edge *data;
  size_t size;
  size_t capacity;
} MaxHeap;

static void heapSwap(Edge *a, Edge *b) {
  Edge tmp = *a;
  *a = *b;
  *b = tmp;
}

static void heapPush(MaxHeap *h, Edge e) {
  if (h->size == h->capacity) {
    size_t newCap = h->capacity ? h->capacity * 2 : 16;
    Edge *n = realloc(h->data, newCap * sizeof(Edge));
    if (!n) {
      return;
    }
    h->data = n;
    h->capacity = newCap;
  }
  size_t idx = h->size++;
  h->data[idx] = e;
  // sift up for max-heap by dist
  while (idx > 0) {
    size_t parent = (idx - 1) / 2;
    if (h->data[parent].dist >= h->data[idx].dist) {
      break;
    }
    heapSwap(&h->data[parent], &h->data[idx]);
    idx = parent;
  }
}

static void heapReplaceRoot(MaxHeap *h, Edge e) {
  if (h->size == 0) {
    heapPush(h, e);
    return;
  }
  h->data[0] = e;
  size_t idx = 0;
  while (1) {
    size_t l = idx * 2 + 1;
    size_t r = idx * 2 + 2;
    size_t largest = idx;
    if (l < h->size && h->data[l].dist > h->data[largest].dist) {
      largest = l;
    }
    if (r < h->size && h->data[r].dist > h->data[largest].dist) {
      largest = r;
    }
    if (largest == idx) {
      break;
    }
    heapSwap(&h->data[idx], &h->data[largest]);
    idx = largest;
  }
}

static int cmpEdge(const void *a, const void *b) {
  const Edge *ea = (const Edge *)a;
  const Edge *eb = (const Edge *)b;
  if (ea->dist < eb->dist) return -1;
  if (ea->dist > eb->dist) return 1;
  return 0;
}

// Union Find
static int findSet(int *parent, int v) {
  if (parent[v] == v) return v;
  parent[v] = findSet(parent, parent[v]);
  return parent[v];
}

static void unionSet(int *parent, int *size, int a, int b) {
  a = findSet(parent, a);
  b = findSet(parent, b);
  if (a == b) return;
  if (size[a] < size[b]) {
    int t = a;
    a = b;
    b = t;
  }
  parent[b] = a;
  size[a] += size[b];
}

unsigned long long day8Solve(const char *data) {
  // Parse points
  size_t capacity = 32;
  size_t count = 0;
  Point3 *pts = malloc(capacity * sizeof(Point3));
  const char *p = data;
  while (*p) {
    while (*p == '\n' || *p == '\r') p++;
    if (*p == '\0') break;
    char *end;
    int x = (int)strtol(p, &end, 10);
    if (end == p || *end != ',') break;
    p = end + 1;
    int y = (int)strtol(p, &end, 10);
    if (end == p || *end != ',') break;
    p = end + 1;
    int z = (int)strtol(p, &end, 10);
    if (end == p) break;
    if (count == capacity) {
      capacity *= 2;
      Point3 *n = realloc(pts, capacity * sizeof(Point3));
      if (!n) {
        free(pts);
        return 0;
      }
      pts = n;
    }
    pts[count++] = (Point3){x, y, z};
    p = end;
    while (*p == ' ' || *p == '\t') p++;
    if (*p == '\n') p++;
  }
  if (count == 0) {
    free(pts);
    return 0;
  }

  size_t limit = (count <= 30) ? 10 : 1000;
  size_t totalPairs = count * (count - 1) / 2;
  if (limit > totalPairs) limit = totalPairs;

  MaxHeap heap = {.data = NULL, .size = 0, .capacity = 0};

  for (size_t i = 0; i < count; ++i) {
    for (size_t j = i + 1; j < count; ++j) {
      long dx = (long)pts[i].x - (long)pts[j].x;
      long dy = (long)pts[i].y - (long)pts[j].y;
      long dz = (long)pts[i].z - (long)pts[j].z;
      unsigned long long dist = (unsigned long long)(dx * dx + dy * dy + dz * dz);
      Edge e = {.dist = dist, .a = (int)i, .b = (int)j};
      if (heap.size < limit) {
        heapPush(&heap, e);
      } else if (dist < heap.data[0].dist) {
        heapReplaceRoot(&heap, e);
      }
    }
  }

  // Gather edges
  Edge *edges = heap.data;
  size_t edgeCount = heap.size;
  qsort(edges, edgeCount, sizeof(Edge), cmpEdge);

  int *parent = malloc(count * sizeof(int));
  int *sizes = malloc(count * sizeof(int));
  if (!parent || !sizes) {
    free(parent);
    free(sizes);
    free(edges);
    free(pts);
    return 0;
  }
  for (size_t i = 0; i < count; ++i) {
    parent[i] = (int)i;
    sizes[i] = 1;
  }

  for (size_t i = 0; i < edgeCount; ++i) {
    unionSet(parent, sizes, edges[i].a, edges[i].b);
  }

  // Compute component sizes
  // reuse sizes array: root sizes up to count
  // collect top3
  unsigned long long top1 = 0, top2 = 0, top3 = 0;
  for (size_t i = 0; i < count; ++i) {
    if (parent[i] == (int)i) {
      unsigned long long s = (unsigned long long)sizes[i];
      if (s > top1) {
        top3 = top2;
        top2 = top1;
        top1 = s;
      } else if (s > top2) {
        top3 = top2;
        top2 = s;
      } else if (s > top3) {
        top3 = s;
      }
    }
  }

  unsigned long long result = top1 * top2 * top3;

  free(parent);
  free(sizes);
  free(edges);
  free(pts);
  return result;
}

unsigned long long day8SolvePartTwo(const char *data) {
  size_t capacity = 32;
  size_t count = 0;
  Point3 *pts = malloc(capacity * sizeof(Point3));
  const char *p = data;
  while (*p) {
    while (*p == '\n' || *p == '\r') p++;
    if (*p == '\0') break;
    char *end;
    int x = (int)strtol(p, &end, 10);
    if (end == p || *end != ',') break;
    p = end + 1;
    int y = (int)strtol(p, &end, 10);
    if (end == p || *end != ',') break;
    p = end + 1;
    int z = (int)strtol(p, &end, 10);
    if (end == p) break;
    if (count == capacity) {
      capacity *= 2;
      Point3 *n = realloc(pts, capacity * sizeof(Point3));
      if (!n) {
        free(pts);
        return 0;
      }
      pts = n;
    }
    pts[count++] = (Point3){x, y, z};
    p = end;
    while (*p == ' ' || *p == '\t') p++;
    if (*p == '\n') p++;
  }
  if (count == 0) {
    free(pts);
    return 0;
  }

  // Prim's algorithm with O(n^2) time, O(n) memory
  unsigned long long *best = malloc(count * sizeof(unsigned long long));
  int *parent = malloc(count * sizeof(int));
  char *inTree = calloc(count, 1);
  if (!best || !parent || !inTree) {
    free(best);
    free(parent);
    free(inTree);
    free(pts);
    return 0;
  }

  const unsigned long long INF = ~(unsigned long long)0;
  for (size_t i = 0; i < count; ++i) {
    best[i] = INF;
    parent[i] = -1;
  }
  best[0] = 0;

  unsigned long long maxEdge = 0;
  int maxA = 0, maxB = 0;

  for (size_t iter = 0; iter < count; ++iter) {
    size_t u = count;
    unsigned long long minDist = INF;
    for (size_t i = 0; i < count; ++i) {
      if (!inTree[i] && best[i] < minDist) {
        minDist = best[i];
        u = i;
      }
    }
    if (u == count) {
      break;
    }
    inTree[u] = 1;
    if (parent[u] != -1 && minDist > maxEdge) {
      maxEdge = minDist;
      maxA = parent[u];
      maxB = (int)u;
    }

    for (size_t v = 0; v < count; ++v) {
      if (inTree[v]) continue;
      long dx = (long)pts[u].x - (long)pts[v].x;
      long dy = (long)pts[u].y - (long)pts[v].y;
      long dz = (long)pts[u].z - (long)pts[v].z;
      unsigned long long dist = (unsigned long long)(dx * dx + dy * dy + dz * dz);
      if (dist < best[v]) {
        best[v] = dist;
        parent[v] = (int)u;
      }
    }
  }

  unsigned long long answer =
      (unsigned long long)pts[maxA].x * (unsigned long long)pts[maxB].x;

  free(best);
  free(parent);
  free(inTree);
  free(pts);
  return answer;
}
