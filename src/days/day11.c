#include "days/day11.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int n;
  int cap;
  char **names;
  int *head;
  int edges;
  int edgeCap;
  int *to;
  int *next;
  int mapCap;
  int *mapId;
} Graph11;

static void graphInit(Graph11 *g) {
  g->n = 0;
  g->cap = 128;
  g->names = (char **)calloc((size_t)g->cap, sizeof(char *));
  g->head = (int *)malloc((size_t)g->cap * sizeof(int));
  for (int i = 0; i < g->cap; ++i) g->head[i] = -1;
  g->edges = 0;
  g->edgeCap = 256;
  g->to = (int *)malloc((size_t)g->edgeCap * sizeof(int));
  g->next = (int *)malloc((size_t)g->edgeCap * sizeof(int));
  g->mapCap = 256;
  g->mapId = (int *)malloc((size_t)g->mapCap * sizeof(int));
  for (int i = 0; i < g->mapCap; ++i) g->mapId[i] = -1;
}

static void graphDestroy(Graph11 *g) {
  for (int i = 0; i < g->n; ++i) free(g->names[i]);
  free(g->names);
  free(g->head);
  free(g->to);
  free(g->next);
  free(g->mapId);
}

static int graphEnsureNodes(Graph11 *g, int need) {
  if (need <= g->cap) return 1;
  int newCap = g->cap;
  while (newCap < need) newCap *= 2;
  char **newNames = (char **)realloc(g->names, (size_t)newCap * sizeof(char *));
  int *newHead = (int *)realloc(g->head, (size_t)newCap * sizeof(int));
  if (!newNames || !newHead) return 0;
  for (int i = g->cap; i < newCap; ++i) {
    newHead[i] = -1;
    newNames[i] = NULL;
  }
  g->names = newNames;
  g->head = newHead;
  g->cap = newCap;
  return 1;
}

static int graphEnsureEdges(Graph11 *g, int need) {
  if (need <= g->edgeCap) return 1;
  int newCap = g->edgeCap;
  while (newCap < need) newCap *= 2;
  int *newTo = (int *)realloc(g->to, (size_t)newCap * sizeof(int));
  int *newNext = (int *)realloc(g->next, (size_t)newCap * sizeof(int));
  if (!newTo || !newNext) return 0;
  g->to = newTo;
  g->next = newNext;
  g->edgeCap = newCap;
  return 1;
}

static unsigned long hashName(const char *s) {
  unsigned long h = 5381UL;
  while (*s) h = ((h << 5) + h) ^ (unsigned char)(*s++);
  return h;
}

static int graphRehash(Graph11 *g) {
  int newCap = g->mapCap * 2;
  int *newMap = (int *)malloc((size_t)newCap * sizeof(int));
  if (!newMap) return 0;
  for (int i = 0; i < newCap; ++i) newMap[i] = -1;
  int mask = newCap - 1;
  for (int id = 0; id < g->n; ++id) {
    unsigned long h = hashName(g->names[id]);
    int pos = (int)(h & (unsigned long)mask);
    while (newMap[pos] != -1) pos = (pos + 1) & mask;
    newMap[pos] = id;
  }
  free(g->mapId);
  g->mapId = newMap;
  g->mapCap = newCap;
  return 1;
}

static int graphFind(Graph11 *g, const char *name) {
  unsigned long h = hashName(name);
  int mask = g->mapCap - 1;
  int pos = (int)(h & (unsigned long)mask);
  for (int step = 0; step < g->mapCap; ++step) {
    int id = g->mapId[pos];
    if (id == -1) return -1;
    if (strcmp(g->names[id], name) == 0) return id;
    pos = (pos + 1) & mask;
  }
  return -1;
}

static int graphAddNode(Graph11 *g, const char *name) {
  int id = graphFind(g, name);
  if (id >= 0) return id;
  if (g->n * 2 >= g->mapCap) {
    if (!graphRehash(g)) return -1;
  }
  if (!graphEnsureNodes(g, g->n + 1)) return -1;
  size_t len = strlen(name);
  char *copy = (char *)malloc(len + 1);
  if (!copy) return -1;
  memcpy(copy, name, len + 1);
  if (!copy) return -1;
  id = g->n++;
  g->names[id] = copy;
  g->head[id] = -1;
  unsigned long h = hashName(name);
  int mask = g->mapCap - 1;
  int pos = (int)(h & (unsigned long)mask);
  while (g->mapId[pos] != -1) pos = (pos + 1) & mask;
  g->mapId[pos] = id;
  return id;
}

static int graphAddEdge(Graph11 *g, int from, int to) {
  if (from < 0 || to < 0) return 0;
  if (!graphEnsureEdges(g, g->edges + 1)) return 0;
  int e = g->edges++;
  g->to[e] = to;
  g->next[e] = g->head[from];
  g->head[from] = e;
  return 1;
}

static unsigned long long dfsCount(int u, int target, const Graph11 *g,
                                   unsigned long long *memo, uint8_t *state) {
  if (state[u] == 2) return memo[u];
  if (state[u] == 1) return 0;  // cycle detected: ignore
  state[u] = 1;
  unsigned long long ways = (u == target) ? 1ULL : 0ULL;
  for (int e = g->head[u]; e != -1; e = g->next[e]) {
    ways += dfsCount(g->to[e], target, g, memo, state);
  }
  state[u] = 2;
  memo[u] = ways;
  return ways;
}

static unsigned long long solvePaths(const char *data) {
  Graph11 g;
  graphInit(&g);

  const char *p = data;
  while (*p) {
    const char *lineStart = p;
    while (*p && *p != '\n') p++;
    size_t len = (size_t)(p - lineStart);
    if (len > 0) {
      char *line = (char *)malloc(len + 1);
      if (!line) {
        graphDestroy(&g);
        return 0;
      }
      memcpy(line, lineStart, len);
      line[len] = '\0';

      char *colon = strchr(line, ':');
      if (colon) {
        *colon = '\0';
        char *lhs = line;
        while (*lhs == ' ' || *lhs == '\t') lhs++;
        char *end = lhs + strlen(lhs);
        while (end > lhs && (end[-1] == ' ' || end[-1] == '\t')) *--end = '\0';
        int fromId = graphAddNode(&g, lhs);

        char *rhs = colon + 1;
        while (*rhs) {
          while (*rhs == ' ' || *rhs == '\t') rhs++;
          if (*rhs == '\0') break;
          char *tok = rhs;
          while (*rhs && *rhs != ' ' && *rhs != '\t') rhs++;
          char saved = *rhs;
          *rhs = '\0';
          int toId = graphAddNode(&g, tok);
          graphAddEdge(&g, fromId, toId);
          *rhs = saved;
        }
      }
      free(line);
    }
    if (*p == '\n') p++;
  }

  int youId = graphFind(&g, "you");
  int outId = graphFind(&g, "out");
  if (youId < 0 || outId < 0) {
    graphDestroy(&g);
    return 0;
  }

  unsigned long long *memo =
      (unsigned long long *)calloc((size_t)g.n, sizeof(unsigned long long));
  uint8_t *state = (uint8_t *)calloc((size_t)g.n, sizeof(uint8_t));
  if (!memo || !state) {
    free(memo);
    free(state);
    graphDestroy(&g);
    return 0;
  }
  unsigned long long ans = dfsCount(youId, outId, &g, memo, state);
  free(memo);
  free(state);
  graphDestroy(&g);
  return ans;
}

unsigned long long day11Solve(const char *data) { return solvePaths(data); }

// Part 2: count paths from svr to out that visit both dac and fft (any order).

static unsigned long long dfsCountMask(int u, int target, const Graph11 *g,
                                       const uint8_t *reqBits, uint8_t reqMask,
                                       unsigned long long *memo, uint8_t *state,
                                       uint8_t mask) {
  size_t idx = ((size_t)u << 2) | mask;
  if (state[idx] == 2) return memo[idx];
  if (state[idx] == 1) return 0;
  uint8_t newMask = (uint8_t)(mask | reqBits[u]);
  if (u == target) {
    unsigned long long ok = ((newMask & reqMask) == reqMask) ? 1ULL : 0ULL;
    state[idx] = 2;
    memo[idx] = ok;
    return ok;
  }
  state[idx] = 1;
  unsigned long long ways = 0;
  for (int e = g->head[u]; e != -1; e = g->next[e]) {
    ways += dfsCountMask(g->to[e], target, g, reqBits, reqMask, memo, state,
                         newMask);
  }
  state[idx] = 2;
  memo[idx] = ways;
  return ways;
}

unsigned long long day11SolvePartTwo(const char *data) {
  Graph11 g;
  graphInit(&g);

  const char *p = data;
  while (*p) {
    const char *lineStart = p;
    while (*p && *p != '\n') p++;
    size_t len = (size_t)(p - lineStart);
    if (len > 0) {
      char *line = (char *)malloc(len + 1);
      if (!line) {
        graphDestroy(&g);
        return 0;
      }
      memcpy(line, lineStart, len);
      line[len] = '\0';

      char *colon = strchr(line, ':');
      if (colon) {
        *colon = '\0';
        char *lhs = line;
        while (*lhs == ' ' || *lhs == '\t') lhs++;
        char *end = lhs + strlen(lhs);
        while (end > lhs && (end[-1] == ' ' || end[-1] == '\t')) *--end = '\0';
        int fromId = graphAddNode(&g, lhs);

        char *rhs = colon + 1;
        while (*rhs) {
          while (*rhs == ' ' || *rhs == '\t') rhs++;
          if (*rhs == '\0') break;
          char *tok = rhs;
          while (*rhs && *rhs != ' ' && *rhs != '\t') rhs++;
          char saved = *rhs;
          *rhs = '\0';
          int toId = graphAddNode(&g, tok);
          graphAddEdge(&g, fromId, toId);
          *rhs = saved;
        }
      }
      free(line);
    }
    if (*p == '\n') p++;
  }

  int start = graphFind(&g, "svr");
  int target = graphFind(&g, "out");
  int dacId = graphFind(&g, "dac");
  int fftId = graphFind(&g, "fft");
  if (start < 0 || target < 0 || dacId < 0 || fftId < 0) {
    graphDestroy(&g);
    return 0;
  }

  uint8_t reqMask = 3u;
  uint8_t *reqBits = (uint8_t *)calloc((size_t)g.n, sizeof(uint8_t));
  if (!reqBits) {
    graphDestroy(&g);
    return 0;
  }
  reqBits[dacId] |= 1u;
  reqBits[fftId] |= 2u;

  size_t states = (size_t)g.n * 4u;
  unsigned long long *memo =
      (unsigned long long *)calloc(states, sizeof(unsigned long long));
  uint8_t *state = (uint8_t *)calloc(states, sizeof(uint8_t));
  if (!memo || !state) {
    free(memo);
    free(state);
    free(reqBits);
    graphDestroy(&g);
    return 0;
  }

  unsigned long long ans =
      dfsCountMask(start, target, &g, reqBits, reqMask, memo, state, 0);

  free(memo);
  free(state);
  free(reqBits);
  graphDestroy(&g);
  return ans;
}
