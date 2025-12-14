#include "day2.h"

#include <stdint.h>
#include <stdlib.h>

static unsigned long long pow10u(int exp) {
  static const unsigned long long pow10[] = {
      1ULL,
      10ULL,
      100ULL,
      1000ULL,
      10000ULL,
      100000ULL,
      1000000ULL,
      10000000ULL,
      100000000ULL,
      1000000000ULL,
      10000000000ULL,
      100000000000ULL,
      1000000000000ULL,
      10000000000000ULL,
      100000000000000ULL,
      1000000000000000ULL,
      10000000000000000ULL,
      100000000000000000ULL,
      1000000000000000000ULL,
  };
  return pow10[exp];
}

static int digitCount(unsigned long long value) {
  int count = 1;
  while (value >= 10ULL) {
    value /= 10ULL;
    count++;
  }
  return count;
}

static int hasSmallerRepeat(unsigned long long value, int digits) {
  char buf[20];
  int idx = digits - 1;
  unsigned long long tmp = value;
  while (idx >= 0) {
    buf[idx] = (char)('0' + (tmp % 10ULL));
    tmp /= 10ULL;
    idx--;
  }

  for (int sub = 1; sub <= digits / 2; ++sub) {
    if (digits % sub != 0) {
      continue;
    }
    int ok = 1;
    for (int i = sub; i < digits; ++i) {
      if (buf[i] != buf[i % sub]) {
        ok = 0;
        break;
      }
    }
    if (ok) {
      return 1;
    }
  }
  return 0;
}

static int maxPow10Index(void) { return 18; }

static int buildMultiplier(int blockDigits, int repeats,
                           unsigned long long *outMultiplier) {
  unsigned long long pow = pow10u(blockDigits);
  unsigned long long multiplier = 0;
  for (int i = 0; i < repeats; ++i) {
    if (multiplier > (UINT64_MAX - 1ULL) / pow) {
      return 0;
    }
    multiplier = multiplier * pow + 1ULL;
  }
  *outMultiplier = multiplier;
  return 1;
}

static unsigned long long day2SumInvalidIds(unsigned long long start,
                                            unsigned long long end) {
  if (start > end) {
    unsigned long long tmp = start;
    start = end;
    end = tmp;
  }

  unsigned long long total = 0;
  int maxDigits = digitCount(end);
  int maxHalf = maxDigits / 2;
  if (maxHalf > 9) {
    maxHalf = 9;
  }

  for (int half = 1; half <= maxHalf; ++half) {
    unsigned long long factor = pow10u(half);
    unsigned long long multiplier = factor + 1ULL;
    unsigned long long minPrefix = pow10u(half - 1);
    unsigned long long maxPrefix = factor - 1ULL;

    unsigned long long prefixStart = (start + multiplier - 1ULL) / multiplier;
    if (prefixStart < minPrefix) {
      prefixStart = minPrefix;
    }

    unsigned long long prefixEnd = end / multiplier;
    if (prefixEnd > maxPrefix) {
      prefixEnd = maxPrefix;
    }

    if (prefixStart > prefixEnd) {
      continue;
    }

    for (unsigned long long prefix = prefixStart; prefix <= prefixEnd;
         ++prefix) {
      unsigned long long candidate = prefix * multiplier;
      if (candidate >= start && candidate <= end) {
        total += candidate;
      }
    }
  }

  return total;
}

unsigned long long day2Solve(const char *data) {
  unsigned long long total = 0;
  const char *p = data;

  while (*p) {
    while (*p == ',' || *p == '\n' || *p == '\r' || *p == ' ' || *p == '\t') {
      p++;
    }
    if (*p == '\0') {
      break;
    }

    char *endPtr;
    unsigned long long rangeStart = strtoull(p, &endPtr, 10);
    if (endPtr == p) {
      break;
    }
    p = endPtr;

    while (*p == ' ') {
      p++;
    }
    if (*p != '-') {
      break;
    }
    p++;

    while (*p == ' ') {
      p++;
    }
    unsigned long long rangeEnd = strtoull(p, &endPtr, 10);
    if (endPtr == p) {
      break;
    }
    p = endPtr;

    total += day2SumInvalidIds(rangeStart, rangeEnd);

    while (*p && *p != ',' && *p != '\n' && *p != '\r') {
      p++;
    }
    if (*p == ',') {
      p++;
    }
  }

  return total;
}

static unsigned long long day2SumInvalidIdsAtLeastTwo(unsigned long long start,
                                                      unsigned long long end) {
  if (start > end) {
    unsigned long long tmp = start;
    start = end;
    end = tmp;
  }

  unsigned long long total = 0;
  int maxDigits = digitCount(end);

  for (int blockDigits = 1;
       blockDigits <= maxDigits && blockDigits <= maxPow10Index(); ++blockDigits) {
    unsigned long long minPrefix = pow10u(blockDigits - 1);
    unsigned long long maxPrefix = pow10u(blockDigits) - 1ULL;
    int maxRepeats = maxDigits / blockDigits;

    for (int repeats = 2; repeats <= maxRepeats; ++repeats) {
      unsigned long long multiplier;
      if (!buildMultiplier(blockDigits, repeats, &multiplier)) {
        break;
      }

      unsigned long long prefixStart = (start + multiplier - 1ULL) / multiplier;
      unsigned long long prefixEnd = end / multiplier;

      if (prefixStart < minPrefix) {
        prefixStart = minPrefix;
      }
      if (prefixEnd > maxPrefix) {
        prefixEnd = maxPrefix;
      }

      if (prefixStart > prefixEnd) {
        continue;
      }

      for (unsigned long long prefix = prefixStart; prefix <= prefixEnd;
           ++prefix) {
        if (hasSmallerRepeat(prefix, blockDigits)) {
          continue;
        }
        if (prefix > UINT64_MAX / multiplier) {
          break;
        }
        unsigned long long candidate = prefix * multiplier;
        if (candidate >= start && candidate <= end) {
          total += candidate;
        }
      }
    }
  }

  return total;
}

unsigned long long day2SolvePartTwo(const char *data) {
  unsigned long long total = 0;
  const char *p = data;

  while (*p) {
    while (*p == ',' || *p == '\n' || *p == '\r' || *p == ' ' || *p == '\t') {
      p++;
    }
    if (*p == '\0') {
      break;
    }

    char *endPtr;
    unsigned long long rangeStart = strtoull(p, &endPtr, 10);
    if (endPtr == p) {
      break;
    }
    p = endPtr;

    while (*p == ' ') {
      p++;
    }
    if (*p != '-') {
      break;
    }
    p++;

    while (*p == ' ') {
      p++;
    }
    unsigned long long rangeEnd = strtoull(p, &endPtr, 10);
    if (endPtr == p) {
      break;
    }
    p = endPtr;

    total += day2SumInvalidIdsAtLeastTwo(rangeStart, rangeEnd);

    while (*p && *p != ',' && *p != '\n' && *p != '\r') {
      p++;
    }
    if (*p == ',') {
      p++;
    }
  }

  return total;
}
