#include "days/day10.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

static double absd(double x) { return x < 0.0 ? -x : x; }

static int popcount64(uint64_t x) { return __builtin_popcountll(x); }

static int parseInt(const char **p) {
  int v = 0;
  while (**p >= '0' && **p <= '9') {
    v = v * 10 + (**p - '0');
    (*p)++;
  }
  return v;
}

static unsigned long long solveLights(const char *line) {
  const char *p = line;
  while (*p && *p != '[') p++;
  if (*p != '[') return 0;
  p++;
  char pattern[64];
  int lights = 0;
  while (*p && *p != ']' && lights < 64) {
    pattern[lights++] = *p++;
  }
  if (*p != ']') return 0;
  p++;

  uint64_t targetMask = 0;
  for (int i = 0; i < lights; ++i) {
    if (pattern[i] == '#') targetMask |= (1ULL << i);
  }

  uint64_t buttons[64];
  int nButtons = 0;
  while (*p) {
    while (*p == ' ' || *p == '\t') p++;
    if (*p == '(') {
      p++;
      uint64_t mask = 0;
      while (*p && *p != ')') {
        if (*p >= '0' && *p <= '9') {
          int idx = parseInt(&p);
          if (idx >= 0 && idx < 64) mask ^= (1ULL << idx);
        } else if (*p == ',') {
          p++;
        } else {
          p++;
        }
      }
      if (*p == ')') p++;
      if (nButtons < 64) buttons[nButtons++] = mask;
    } else {
      p++;
    }
  }

  if (lights == 0 || nButtons == 0) return 0;

  uint64_t rows[64] = {0};
  uint8_t rhs[64] = {0};
  for (int r = 0; r < lights; ++r) {
    rhs[r] = (uint8_t)((targetMask >> r) & 1u);
    for (int c = 0; c < nButtons; ++c) {
      if (buttons[c] & (1ULL << r)) rows[r] |= (1ULL << c);
    }
  }

  int pivotCols[64];
  int pivotRows[64];
  int pivotCount = 0;
  int rptr = 0;
  for (int col = 0; col < nButtons && rptr < lights; ++col) {
    int sel = -1;
    for (int r = rptr; r < lights; ++r) {
      if (rows[r] & (1ULL << col)) {
        sel = r;
        break;
      }
    }
    if (sel == -1) continue;
    if (sel != rptr) {
      uint64_t tr = rows[rptr];
      rows[rptr] = rows[sel];
      rows[sel] = tr;
      uint8_t rr = rhs[rptr];
      rhs[rptr] = rhs[sel];
      rhs[sel] = rr;
    }
    pivotCols[pivotCount] = col;
    pivotRows[pivotCount] = rptr;
    pivotCount++;
    for (int r = 0; r < lights; ++r) {
      if (r != rptr && (rows[r] & (1ULL << col))) {
        rows[r] ^= rows[rptr];
        rhs[r] ^= rhs[rptr];
      }
    }
    rptr++;
  }

  for (int r = pivotCount; r < lights; ++r) {
    if (rows[r] == 0 && rhs[r]) return 0;
  }

  int isPivot[64] = {0};
  for (int i = 0; i < pivotCount; ++i) isPivot[pivotCols[i]] = 1;
  int freeCols[64];
  int freeCount = 0;
  for (int c = 0; c < nButtons; ++c) {
    if (!isPivot[c]) freeCols[freeCount++] = c;
  }

  unsigned long long best = UINT64_MAX;
  int thresh = 22;
  if (freeCount <= thresh) {
    size_t combos = 1ULL << freeCount;
    uint64_t freeMasks[64];
    for (int i = 0; i < freeCount; ++i) freeMasks[i] = (1ULL << freeCols[i]);
    for (size_t mask = 0; mask < combos; ++mask) {
      uint64_t sol = 0;
      size_t m = mask;
      for (int i = 0; i < freeCount; ++i, m >>= 1) {
        if (m & 1ULL) sol |= freeMasks[i];
      }
      for (int idx = pivotCount - 1; idx >= 0; --idx) {
        int prow = pivotRows[idx];
        int pcol = pivotCols[idx];
        uint64_t rowMask = rows[prow] & ~(1ULL << pcol);
        uint8_t sum = (uint8_t)((unsigned int)popcount64(rowMask & sol) & 1u);
        uint8_t val = rhs[prow] ^ sum;
        if (val) sol |= (1ULL << pcol);
        else sol &= ~(1ULL << pcol);
      }
      unsigned long long w = (unsigned long long)popcount64(sol);
      if (w < best) best = w;
    }
  } else {
    uint64_t sol = 0;
    for (int idx = pivotCount - 1; idx >= 0; --idx) {
      int prow = pivotRows[idx];
      int pcol = pivotCols[idx];
      uint64_t rowMask = rows[prow] & ~(1ULL << pcol);
      uint8_t sum = (uint8_t)((unsigned int)popcount64(rowMask & sol) & 1u);
      uint8_t val = rhs[prow] ^ sum;
      if (val) sol |= (1ULL << pcol);
    }
    best = (unsigned long long)popcount64(sol);
  }

  return best == UINT64_MAX ? 0 : best;
}

unsigned long long day10Solve(const char *data) {
  unsigned long long total = 0;
  const char *p = data;
  while (*p) {
    const char *start = p;
    while (*p && *p != '\n') p++;
    size_t len = (size_t)(p - start);
    if (len > 0) {
      char *line = malloc(len + 1);
      if (!line) return total;
      memcpy(line, start, len);
      line[len] = '\0';
      total += solveLights(line);
      free(line);
    }
    if (*p == '\n') p++;
  }
  return total;
}

typedef struct {
  int counters;
  int buttons;
  uint8_t a[64][64];
  uint32_t target[64];
} Machine10;

static int buildMachine(const char *line, Machine10 *m) {
  memset(m, 0, sizeof(*m));

  const char *q = line;
  while (*q && *q != '{') q++;
  if (*q == '{') {
    q++;
    while (*q && *q != '}' && m->counters < 64) {
      if (*q >= '0' && *q <= '9') {
        int v = parseInt(&q);
        if (v < 0) v = 0;
        m->target[m->counters++] = (uint32_t)v;
      } else {
        q++;
      }
    }
  }
  q = line;
  while (*q) {
    while (*q == ' ' || *q == '\t') q++;
    if (*q == '(') {
      q++;
      uint64_t mask = 0;
      while (*q && *q != ')') {
        if (*q >= '0' && *q <= '9') {
          int idx = parseInt(&q);
          if (idx >= 0 && idx < 64) mask |= (1ULL << idx);
        } else if (*q == ',') {
          q++;
        } else {
          q++;
        }
      }
      if (*q == ')') q++;
      if (m->buttons < 64) {
        for (int i = 0; i < m->counters; ++i) {
          if (mask & (1ULL << i)) m->a[i][m->buttons] = 1u;
        }
        m->buttons++;
      }
    } else {
      q++;
    }
  }
  return (m->counters > 0 && m->buttons > 0);
}

// Part 2: LP relaxation (two-phase simplex) + branch-and-bound.

#define D10_LP_MAXR 64
#define D10_LP_MAXC 256

typedef struct {
  int rows;
  int cols;     // non-art vars (x + branch slacks)
  int origCols; // original x vars (buttons)
  double A[D10_LP_MAXR][D10_LP_MAXC];
  double b[D10_LP_MAXR];
} D10Problem;

typedef struct {
  int feasible;
  double sumX;
  double x[D10_LP_MAXC];
} D10LpSol;

static void d10ProbInit(D10Problem *p, const Machine10 *m) {
  memset(p, 0, sizeof(*p));
  p->rows = m->counters;
  p->cols = m->buttons;
  p->origCols = m->buttons;
  for (int i = 0; i < m->counters; ++i) {
    p->b[i] = (double)m->target[i];
    for (int j = 0; j < m->buttons; ++j) p->A[i][j] = (double)m->a[i][j];
  }
}

static int d10ProbAddLe(D10Problem *p, int var, double bound) {
  if (p->rows + 1 >= D10_LP_MAXR) return 0;
  if (p->cols + 1 >= D10_LP_MAXC) return 0;
  int r = p->rows++;
  int slack = p->cols++;
  for (int j = 0; j < p->cols; ++j) p->A[r][j] = 0.0;
  p->A[r][var] = 1.0;
  p->A[r][slack] = 1.0;
  p->b[r] = bound;
  return 1;
}

static int d10ProbAddGe(D10Problem *p, int var, double bound) {
  if (p->rows + 1 >= D10_LP_MAXR) return 0;
  if (p->cols + 1 >= D10_LP_MAXC) return 0;
  int r = p->rows++;
  int slack = p->cols++;
  for (int j = 0; j < p->cols; ++j) p->A[r][j] = 0.0;
  p->A[r][var] = 1.0;
  p->A[r][slack] = -1.0;
  p->b[r] = bound;
  return 1;
}

static void d10Pivot(double tab[D10_LP_MAXR][D10_LP_MAXC], int rows, int rhsCol,
                     int basic[D10_LP_MAXR], int pr, int pc) {
  double inv = 1.0 / tab[pr][pc];
  for (int j = 0; j <= rhsCol; ++j) tab[pr][j] *= inv;
  for (int i = 0; i <= rows; ++i) {
    if (i == pr) continue;
    double f = tab[i][pc];
    if (absd(f) < 1e-12) continue;
    for (int j = 0; j <= rhsCol; ++j) tab[i][j] -= f * tab[pr][j];
  }
  basic[pr] = pc;
}

static void d10BuildObj(double tab[D10_LP_MAXR][D10_LP_MAXC], int rows,
                        int rhsCol, const double *cost,
                        const int basic[D10_LP_MAXR]) {
  for (int j = 0; j <= rhsCol; ++j) tab[rows][j] = 0.0;
  for (int j = 0; j < rhsCol; ++j) tab[rows][j] = cost[j];
  tab[rows][rhsCol] = 0.0;
  for (int i = 0; i < rows; ++i) {
    int bv = basic[i];
    double cb = cost[bv];
    if (absd(cb) < 1e-12) continue;
    for (int j = 0; j <= rhsCol; ++j) tab[rows][j] -= cb * tab[i][j];
  }
}

static void d10SimplexMax(double tab[D10_LP_MAXR][D10_LP_MAXC], int rows,
                          int rhsCol, int enterCols,
                          int basic[D10_LP_MAXR]) {
  const double EPS = 1e-9;
  while (1) {
    int enter = -1;
    for (int j = 0; j < enterCols; ++j) {
      if (tab[rows][j] > EPS) {
        enter = j;
        break;
      }
    }
    if (enter == -1) return;
    int leave = -1;
    double best = 0.0;
    for (int i = 0; i < rows; ++i) {
      double a = tab[i][enter];
      if (a > EPS) {
        double ratio = tab[i][rhsCol] / a;
        if (leave == -1 || ratio < best - EPS) {
          leave = i;
          best = ratio;
        }
      }
    }
    if (leave == -1) return;
    d10Pivot(tab, rows, rhsCol, basic, leave, enter);
  }
}

static int d10SolveLpRelax(const D10Problem *p, D10LpSol *out) {
  const double EPS = 1e-8;
  int m = p->rows;
  int n = p->cols;
  if (m <= 0 || n <= 0) return 0;
  int art = m;
  int total = n + art;
  int rhsCol = total;
  if (rhsCol + 1 >= D10_LP_MAXC) return 0;

  static double tab[D10_LP_MAXR][D10_LP_MAXC];
  static int basic[D10_LP_MAXR];
  static double cost[D10_LP_MAXC];

  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) tab[i][j] = p->A[i][j];
    for (int j = 0; j < art; ++j) tab[i][n + j] = (j == i) ? 1.0 : 0.0;
    tab[i][rhsCol] = p->b[i];
    basic[i] = n + i;
  }

  // Phase 1: maximize -sum(artificial) == minimize sum(artificial).
  for (int j = 0; j < total; ++j) cost[j] = 0.0;
  for (int j = n; j < total; ++j) cost[j] = -1.0;
  d10BuildObj(tab, m, rhsCol, cost, basic);
  d10SimplexMax(tab, m, rhsCol, total, basic);
  if (tab[m][rhsCol] < -EPS) {
    out->feasible = 0;
    return 1;
  }

  // Pivot artificials out; drop redundant rows.
  for (int i = 0; i < m; ++i) {
    if (basic[i] < n) continue;
    if (absd(tab[i][rhsCol]) > EPS) {
      out->feasible = 0;
      return 1;
    }
    int pivotCol = -1;
    for (int j = 0; j < n; ++j) {
      if (absd(tab[i][j]) > EPS) {
        pivotCol = j;
        break;
      }
    }
    if (pivotCol != -1) {
      d10Pivot(tab, m, rhsCol, basic, i, pivotCol);
    } else {
      if (i != m - 1) {
        for (int j = 0; j <= rhsCol; ++j) {
          double tmp = tab[i][j];
          tab[i][j] = tab[m - 1][j];
          tab[m - 1][j] = tmp;
        }
        int tb = basic[i];
        basic[i] = basic[m - 1];
        basic[m - 1] = tb;
      }
      m--;
      i--;
    }
  }

  // Phase 2: maximize -sum(x) (x = original button press variables).
  for (int j = 0; j < total; ++j) cost[j] = 0.0;
  for (int j = 0; j < p->origCols; ++j) cost[j] = -1.0;
  d10BuildObj(tab, m, rhsCol, cost, basic);
  d10SimplexMax(tab, m, rhsCol, n, basic);

  for (int j = 0; j < n; ++j) out->x[j] = 0.0;
  for (int i = 0; i < m; ++i) {
    int bv = basic[i];
    if (bv < n) out->x[bv] = tab[i][rhsCol];
  }
  out->feasible = 1;
  double sum = 0.0;
  for (int j = 0; j < p->origCols; ++j) sum += out->x[j];
  out->sumX = sum;
  return 1;
}

static int d10IntegerFeasible(const D10Problem *p, const double *x,
                              long long *xi) {
  for (int j = 0; j < p->cols; ++j) {
    double v = x[j];
    if (v < -1e-7) return 0;
    long long r = (long long)(v + 0.5);
    if (absd(v - (double)r) > 1e-6) return 0;
    if (r < 0) return 0;
    xi[j] = r;
  }
  for (int i = 0; i < p->rows; ++i) {
    long long s = 0;
    for (int j = 0; j < p->cols; ++j) {
      double a = p->A[i][j];
      if (a == 0.0) continue;
      s += (long long)a * xi[j];
    }
    if (s != (long long)(p->b[i] + 0.5)) return 0;
  }
  return 1;
}

static void d10BnB(const D10Problem *p, double *best) {
  D10LpSol lp;
  if (!d10SolveLpRelax(p, &lp) || !lp.feasible) return;
  if (lp.sumX >= *best - 1e-9) return;

  int frac = -1;
  double fracAmt = 0.0;
  for (int j = 0; j < p->origCols; ++j) {
    double v = lp.x[j];
    double f = absd(v - (double)((long long)(v + 0.5)));
    if (f > 1e-6 && f > fracAmt) {
      fracAmt = f;
      frac = j;
    }
  }

  if (frac == -1) {
    long long xi[D10_LP_MAXC];
    if (d10IntegerFeasible(p, lp.x, xi)) {
      if (lp.sumX < *best) *best = lp.sumX;
    }
    return;
  }

  double v = lp.x[frac];
  double lo = (double)((long long)v);
  double hi = lo + 1.0;

  D10Problem left = *p;
  if (d10ProbAddLe(&left, frac, lo)) d10BnB(&left, best);

  D10Problem right = *p;
  if (d10ProbAddGe(&right, frac, hi)) d10BnB(&right, best);
}

static int solveMachinePart2Fast(const Machine10 *m) {
  D10Problem p;
  d10ProbInit(&p, m);
  double best = 1e100;
  d10BnB(&p, &best);
  if (best > 1e90) return -1;
  return (int)(best + 0.5);
}

unsigned long long day10SolvePartTwo(const char *data) {
  unsigned long long total = 0;
  const char *p = data;
  while (*p) {
    const char *start = p;
    while (*p && *p != '\n') p++;
    size_t len = (size_t)(p - start);
    if (len == 0) {
      if (*p == '\n') p++;
      continue;
    }
    char *line = malloc(len + 1);
    if (!line) return total;
    memcpy(line, start, len);
    line[len] = '\0';

    Machine10 mach;
    if (buildMachine(line, &mach)) {
      int best = solveMachinePart2Fast(&mach);
      if (best >= 0) total += (unsigned long long)best;
    }

    free(line);
    if (*p == '\n') p++;
  }
  return total;
}
