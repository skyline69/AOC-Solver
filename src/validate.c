#include "validate.h"

#include "tools.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void setErr(char *buf, size_t n, const char *msg) {
  if (n == 0) {
    return;
  }
  if (msg) {
    snprintf(buf, n, "%s", msg);
  } else {
    buf[0] = '\0';
  }
}

static int parseUnsigned(const char **p, unsigned long long *out) {
  char *end;
  unsigned long long v = strtoull(*p, &end, 10);
  if (end == *p) {
    return 0;
  }
  *out = v;
  *p = end;
  return 1;
}

static int parseSigned(const char **p, long *out) {
  char *end;
  long v = strtol(*p, &end, 10);
  if (end == *p) {
    return 0;
  }
  *out = v;
  *p = end;
  return 1;
}

static int validateDay1(const char *data, size_t size, char *err,
                        size_t errSize) {
  if (size == 0) {
    setErr(err, errSize, "Day 1 input is empty.");
    return 0;
  }
  int moves = 0;
  const char *p = data;
  while (*p) {
    char dir = *p++;
    if (dir != 'L' && dir != 'R') {
      setErr(err, errSize, "Day 1 expects lines like 'L10' or 'R3'.");
      return 0;
    }
    if (!isDigit(*p)) {
      setErr(err, errSize, "Day 1 move missing distance digits.");
      return 0;
    }
    while (isDigit(*p)) {
      p++;
    }
    moves++;
    if (*p == '\r') {
      p++;
    }
    if (*p == '\n') {
      p++;
    } else if (*p == '\0') {
      break;
    } else {
      setErr(err, errSize, "Unexpected character after Day 1 move.");
      return 0;
    }
  }
  if (moves == 0) {
    setErr(err, errSize, "Day 1 input has no moves.");
    return 0;
  }
  return 1;
}

static int validateDay2(const char *data, size_t size, char *err,
                        size_t errSize) {
  (void)size;
  size_t ranges = 0;
  const char *p = data;
  while (*p) {
    while (*p == ',' || *p == '\n' || *p == '\r' || *p == ' ' || *p == '\t') {
      p++;
    }
    if (*p == '\0') {
      break;
    }
    unsigned long long start, end;
    if (!parseUnsigned(&p, &start)) {
      setErr(err, errSize, "Day 2 range is missing a start number.");
      return 0;
    }
    while (*p == ' ') {
      p++;
    }
    if (*p != '-') {
      setErr(err, errSize, "Day 2 ranges must use a '-' separator.");
      return 0;
    }
    p++;
    while (*p == ' ') {
      p++;
    }
    if (!parseUnsigned(&p, &end)) {
      setErr(err, errSize, "Day 2 range is missing an end number.");
      return 0;
    }
    ranges++;
    while (*p && *p != ',' && *p != '\n' && *p != '\r') {
      if (*p != ' ' && *p != '\t') {
        setErr(err, errSize, "Unexpected character inside Day 2 range list.");
        return 0;
      }
      p++;
    }
    if (*p == ',') {
      p++;
    }
  }
  if (ranges == 0) {
    setErr(err, errSize, "Day 2 input contains no ranges.");
    return 0;
  }
  return 1;
}

static int validateDay3(const char *data, size_t size, char *err,
                        size_t errSize) {
  if (size == 0) {
    setErr(err, errSize, "Day 3 input is empty.");
    return 0;
  }
  int digits = 0;
  for (const char *p = data; *p; ++p) {
    if (*p == '\n') {
      continue;
    }
    if (*p == '\r') {
      continue;
    }
    if (*p >= '0' && *p <= '9') {
      digits++;
      continue;
    }
    setErr(err, errSize, "Day 3 expects only digits per line.");
    return 0;
  }
  if (digits == 0) {
    setErr(err, errSize, "Day 3 input has no digits.");
    return 0;
  }
  return 1;
}

static int validateDay4(const char *data, size_t size, char *err,
                        size_t errSize) {
  if (size == 0) {
    setErr(err, errSize, "Day 4 input is empty.");
    return 0;
  }
  size_t atCount = 0;
  for (const char *p = data; *p; ++p) {
    if (*p == '@') {
      atCount++;
    } else if (*p == '.' || *p == '\n' || *p == '\r') {
      continue;
    } else {
      setErr(err, errSize, "Day 4 grid may only contain '.' and '@'.");
      return 0;
    }
  }
  if (atCount == 0) {
    setErr(err, errSize, "Day 4 grid has no '@' cells.");
    return 0;
  }
  return 1;
}

static int validateDay5(const char *data, size_t size, char *err,
                        size_t errSize) {
  (void)size;
  const char *p = data;
  size_t ranges = 0;
  int sawSeparator = 0;

  while (*p) {
    if (*p == '\n') {
      if (*(p + 1) == '\n') {
        p += 2;
        sawSeparator = 1;
        break;
      }
      p++;
      continue;
    }
    if (*p == '\r') {
      p++;
      continue;
    }

    while (*p == ' ') {
      p++;
    }
    unsigned long long start, end;
    if (!parseUnsigned(&p, &start)) {
      setErr(err, errSize, "Day 5 range start is invalid.");
      return 0;
    }
    while (*p == ' ') {
      p++;
    }
    if (*p != '-') {
      setErr(err, errSize, "Day 5 ranges must use a '-' separator.");
      return 0;
    }
    p++;
    while (*p == ' ') {
      p++;
    }
    if (!parseUnsigned(&p, &end)) {
      setErr(err, errSize, "Day 5 range end is invalid.");
      return 0;
    }
    ranges++;
    while (*p && *p != '\n' && *p != '\r') {
      if (*p != ' ' && *p != '\t') {
        setErr(err, errSize,
               "Unexpected character on a Day 5 range line.");
        return 0;
      }
      p++;
    }
    if (*p == '\r') {
      p++;
    }
    if (*p == '\n') {
      if (*(p + 1) == '\n') {
        p += 2;
        sawSeparator = 1;
        break;
      }
      p++;
    }
  }

  if (!sawSeparator) {
    setErr(err, errSize,
           "Day 5 input needs a blank line between ranges and IDs.");
    return 0;
  }
  if (ranges == 0) {
    setErr(err, errSize, "Day 5 input has no ranges before the blank line.");
    return 0;
  }

  size_t ids = 0;
  while (*p) {
    while (*p == '\n' || *p == '\r' || *p == ' ' || *p == '\t') {
      p++;
    }
    if (*p == '\0') {
      break;
    }
    unsigned long long id;
    if (!parseUnsigned(&p, &id)) {
      setErr(err, errSize, "Day 5 ID list contains an invalid number.");
      return 0;
    }
    ids++;
    while (*p == ' ' || *p == '\t') {
      p++;
    }
    if (*p == '\r') {
      p++;
      if (*p == '\n') {
        p++;
      }
    } else if (*p == '\n') {
      p++;
    } else if (*p == '\0') {
      break;
    } else {
      setErr(err, errSize, "Unexpected character after a Day 5 ID.");
      return 0;
    }
  }

  if (ids == 0) {
    setErr(err, errSize, "Day 5 input has no ticket IDs after the blank line.");
    return 0;
  }
  return 1;
}

static int validateDay6(const char *data, size_t size, char *err,
                        size_t errSize) {
  if (size == 0) {
    setErr(err, errSize, "Day 6 input is empty.");
    return 0;
  }
  const char *p = data;
  int sawNonEmpty = 0;
  int hasDigit = 0;
  int lastLineHasOp = 0;
  while (*p) {
    const char *start = p;
    while (*p && *p != '\n') {
      p++;
    }
    size_t len = (size_t)(p - start);
    int nonSpace = 0;
    int lineHasOp = 0;
    int lineHasDigit = 0;
    for (size_t i = 0; i < len; ++i) {
      char c = start[i];
      if (c == '+' || c == '*') {
        nonSpace = 1;
        lineHasOp = 1;
      } else if (c >= '0' && c <= '9') {
        nonSpace = 1;
        lineHasDigit = 1;
      } else if (c == ' ' || c == '\t') {
        continue;
      } else {
        setErr(err, errSize,
               "Day 6 lines may only contain digits, '+', '*' and spaces.");
        return 0;
      }
    }
    if (nonSpace) {
      sawNonEmpty = 1;
      if (lineHasDigit) {
        hasDigit = 1;
      }
      lastLineHasOp = lineHasOp;
    }
    if (*p == '\n') {
      p++;
    } else {
      break;
    }
  }
  if (!sawNonEmpty) {
    setErr(err, errSize, "Day 6 input has no content.");
    return 0;
  }
  if (!lastLineHasOp) {
    setErr(err, errSize,
           "Day 6 expects the last non-empty row to hold operators (+ or *).");
    return 0;
  }
  if (!hasDigit) {
    setErr(err, errSize, "Day 6 input contains no numbers.");
    return 0;
  }
  return 1;
}

static int validateDay7(const char *data, size_t size, char *err,
                        size_t errSize) {
  if (size == 0) {
    setErr(err, errSize, "Day 7 input is empty.");
    return 0;
  }
  int hasStart = 0;
  for (const char *p = data; *p; ++p) {
    if (*p == 'S') {
      hasStart = 1;
    } else if (*p == '.' || *p == '^' || *p == '\n' || *p == '\r') {
      continue;
    } else {
      setErr(err, errSize,
             "Day 7 grid may only contain '.', '^', 'S' and newlines.");
      return 0;
    }
  }
  if (!hasStart) {
    setErr(err, errSize, "Day 7 grid is missing the start marker 'S'.");
    return 0;
  }
  return 1;
}

static int validateDay8(const char *data, size_t size, char *err,
                        size_t errSize) {
  (void)size;
  const char *p = data;
  size_t rows = 0;
  while (*p) {
    while (*p == '\n' || *p == '\r') {
      p++;
    }
    if (*p == '\0') {
      break;
    }
    long x, y, z;
    if (!parseSigned(&p, &x)) {
      setErr(err, errSize,
             "Day 8 expects lines like '1,2,3' with integers.");
      return 0;
    }
    if (*p != ',') {
      setErr(err, errSize, "Day 8 points must separate coordinates with ','.");
      return 0;
    }
    p++;
    if (!parseSigned(&p, &y)) {
      setErr(err, errSize, "Day 8 point is missing the second coordinate.");
      return 0;
    }
    if (*p != ',') {
      setErr(err, errSize, "Day 8 points must separate coordinates with ','.");
      return 0;
    }
    p++;
    if (!parseSigned(&p, &z)) {
      setErr(err, errSize, "Day 8 point is missing the third coordinate.");
      return 0;
    }
    rows++;
    while (*p == ' ' || *p == '\t') {
      p++;
    }
    if (*p == '\n') {
      p++;
    } else if (*p == '\r') {
      p++;
      if (*p == '\n') {
        p++;
      }
    } else if (*p == '\0') {
      break;
    } else {
      setErr(err, errSize, "Unexpected character after a Day 8 point.");
      return 0;
    }
  }
  if (rows == 0) {
    setErr(err, errSize, "Day 8 input has no 3D points.");
    return 0;
  }
  return 1;
}

static int validateDay9(const char *data, size_t size, char *err,
                        size_t errSize) {
  (void)size;
  const char *p = data;
  size_t rows = 0;
  while (*p) {
    while (*p == '\n' || *p == '\r') {
      p++;
    }
    if (*p == '\0') {
      break;
    }
    long x, y;
    if (!parseSigned(&p, &x)) {
      setErr(err, errSize,
             "Day 9 expects lines like '1,2' with integers.");
      return 0;
    }
    if (*p != ',') {
      setErr(err, errSize, "Day 9 points must separate coordinates with ','.");
      return 0;
    }
    p++;
    if (!parseSigned(&p, &y)) {
      setErr(err, errSize, "Day 9 point is missing the second coordinate.");
      return 0;
    }
    rows++;
    while (*p == ' ' || *p == '\t') {
      p++;
    }
    if (*p == '\n') {
      p++;
    } else if (*p == '\r') {
      p++;
      if (*p == '\n') {
        p++;
      }
    } else if (*p == '\0') {
      break;
    } else {
      setErr(err, errSize, "Unexpected character after a Day 9 point.");
      return 0;
    }
  }
  if (rows == 0) {
    setErr(err, errSize, "Day 9 input has no 2D points.");
    return 0;
  }
  return 1;
}

static int validateNumberList(const char *start, const char *end) {
  int numbers = 0;
  const char *p = start;
  while (p < end) {
    while (p < end && (*p == ' ' || *p == '\t' || *p == ',')) {
      p++;
    }
    if (p >= end) {
      break;
    }
    if (*p < '0' || *p > '9') {
      return 0;
    }
    while (p < end && *p >= '0' && *p <= '9') {
      p++;
    }
    numbers++;
    while (p < end && (*p == ' ' || *p == '\t')) {
      p++;
    }
    if (p < end && *p == ',') {
      p++;
    }
  }
  return numbers > 0;
}

static int validateDay10(const char *data, size_t size, char *err,
                         size_t errSize) {
  if (size == 0) {
    setErr(err, errSize, "Day 10 input is empty.");
    return 0;
  }
  const char *p = data;
  size_t lines = 0;
  while (*p) {
    const char *start = p;
    while (*p && *p != '\n') {
      p++;
    }
    size_t len = (size_t)(p - start);
    if (len > 0) {
      lines++;
      const char *lb = memchr(start, '[', len);
      const char *rb = memchr(start, ']', len);
      if (!lb || !rb || lb > rb || rb == lb + 1) {
        setErr(err, errSize,
               "Day 10 lines must start with a [pattern] of '.' and '#'.");
        return 0;
      }
      for (const char *q = lb + 1; q < rb; ++q) {
        if (*q != '.' && *q != '#') {
          setErr(err, errSize,
                 "Day 10 pattern inside [] may only contain '.' or '#'.");
          return 0;
        }
      }

      int parenGroups = 0;
      const char *q = start;
      const char *lineEnd = start + len;
      while (q < lineEnd) {
        if (*q == '(') {
          const char *endParen = q + 1;
          while (endParen < lineEnd && *endParen != ')') {
            endParen++;
          }
          if (endParen >= lineEnd || *endParen != ')') {
            setErr(err, errSize,
                   "Day 10 expects ')' to close each '(' group.");
            return 0;
          }
          if (!validateNumberList(q + 1, endParen)) {
            setErr(err, errSize,
                   "Day 10 button groups must list comma-separated numbers.");
            return 0;
          }
          parenGroups++;
          q = endParen + 1;
          continue;
        }
        q++;
      }
      if (parenGroups == 0) {
        setErr(err, errSize,
               "Day 10 lines need at least one '(...)' button group.");
        return 0;
      }

      const char *braceOpen = memchr(start, '{', len);
      const char *braceClose = NULL;
      if (braceOpen) {
        const char *s = start + len;
        while (s > braceOpen) {
          s--;
          if (*s == '}') {
            braceClose = s;
            break;
          }
        }
      }
      if (!braceOpen || !braceClose || braceClose <= braceOpen + 1) {
        setErr(err, errSize,
               "Day 10 lines must end with '{...}' target values.");
        return 0;
      }
      if (!validateNumberList(braceOpen + 1, braceClose)) {
        setErr(err, errSize,
               "Day 10 target list inside {} must be comma-separated numbers.");
        return 0;
      }
    }
    if (*p == '\n') {
      p++;
    }
  }
  if (lines == 0) {
    setErr(err, errSize, "Day 10 input has no lines.");
    return 0;
  }
  return 1;
}

static int isNameChar(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static int validateDay11(const char *data, size_t size, char *err,
                         size_t errSize) {
  if (size == 0) {
    setErr(err, errSize, "Day 11 input is empty.");
    return 0;
  }
  const char *p = data;
  size_t edges = 0;
  while (*p) {
    const char *start = p;
    while (*p && *p != '\n') {
      p++;
    }
    size_t len = (size_t)(p - start);
    if (len > 0) {
      const char *colon = memchr(start, ':', len);
      if (!colon || colon == start) {
        setErr(err, errSize,
               "Day 11 lines must look like 'node: neighbor neighbor'.");
        return 0;
      }
      for (const char *q = start; q < colon; ++q) {
        if (!isNameChar(*q)) {
          setErr(err, errSize,
                 "Day 11 node names must be alphabetic tokens.");
          return 0;
        }
      }
      int neighbors = 0;
      const char *q = colon + 1;
      while (q < start + len) {
        while (q < start + len && (*q == ' ' || *q == '\t')) {
          q++;
        }
        if (q >= start + len) {
          break;
        }
        const char *tok = q;
        while (q < start + len && *q != ' ' && *q != '\t') {
          if (!isNameChar(*q)) {
            setErr(err, errSize,
                   "Day 11 neighbor names must be alphabetic tokens.");
            return 0;
          }
          q++;
        }
        if (q > tok) {
          neighbors++;
        }
      }
      if (neighbors == 0) {
        setErr(err, errSize,
               "Day 11 lines need at least one neighbor after ':'.");
        return 0;
      }
      edges += (size_t)neighbors;
    }
    if (*p == '\n') {
      p++;
    }
  }
  if (edges == 0) {
    setErr(err, errSize, "Day 11 input lists no edges.");
    return 0;
  }
  return 1;
}

typedef struct {
  const char *ptr;
  size_t len;
} LineView;

static int validateDay12(const char *data, size_t size, char *err,
                         size_t errSize) {
  if (size == 0) {
    setErr(err, errSize, "Day 12 input is empty.");
    return 0;
  }
  // First pass: collect lines
  size_t lineCount = 0;
  for (const char *p = data; *p; ++p) {
    if (*p == '\n') {
      lineCount++;
    }
  }
  lineCount++; // last line

  LineView *lines = malloc(lineCount * sizeof(LineView));
  if (!lines) {
    setErr(err, errSize, "Allocation failure while validating Day 12 input.");
    return 0;
  }

  size_t idx = 0;
  const char *p = data;
  while (*p) {
    const char *start = p;
    while (*p && *p != '\n') {
      p++;
    }
    lines[idx].ptr = start;
    lines[idx].len = (size_t)(p - start);
    idx++;
    if (*p == '\n') {
      p++;
    }
  }
  if (idx < lineCount) {
    lines[idx].ptr = p;
    lines[idx].len = 0;
  }

  size_t lineIdx = 0;
  int shapeId = 0;
  int shapeCount = 0;

  // Parse shapes
  while (lineIdx < lineCount) {
    while (lineIdx < lineCount && lines[lineIdx].len == 0) {
      lineIdx++;
    }
    if (lineIdx >= lineCount) {
      break;
    }
    const LineView *lv = &lines[lineIdx];
    const char *colon = memchr(lv->ptr, ':', lv->len);
    const char *xposHeader = memchr(lv->ptr, 'x', lv->len);
    if (!colon || (xposHeader && xposHeader < colon)) {
      break; // likely start of regions
    }
    // Header should be "<id>:"
    char headerBuf[16];
    if (lv->len >= sizeof(headerBuf)) {
      setErr(err, errSize, "Day 12 shape header is too long.");
      free(lines);
      return 0;
    }
    memcpy(headerBuf, lv->ptr, lv->len);
    headerBuf[lv->len] = '\0';
    int parsedId = atoi(headerBuf);
    if (parsedId != shapeId) {
      setErr(err, errSize, "Day 12 shape IDs must start at 0 and increment.");
      free(lines);
      return 0;
    }
    shapeId++;
    lineIdx++;

    for (const char *q = colon + 1; q < lv->ptr + lv->len; ++q) {
      if (*q != ' ' && *q != '\t') {
        setErr(err, errSize,
               "Day 12 shape header should be just '<id>:' with no extras.");
        free(lines);
        return 0;
      }
    }

    int rows = 0;
    while (lineIdx < lineCount && lines[lineIdx].len > 0 &&
           memchr(lines[lineIdx].ptr, ':', lines[lineIdx].len) == NULL &&
           memchr(lines[lineIdx].ptr, 'x', lines[lineIdx].len) == NULL) {
      const LineView *row = &lines[lineIdx];
      for (size_t i = 0; i < row->len; ++i) {
        char c = row->ptr[i];
        if (c != '.' && c != '#') {
          setErr(err, errSize,
                 "Day 12 shape rows must only contain '.' or '#'.");
          free(lines);
          return 0;
        }
      }
      rows++;
      lineIdx++;
    }
    if (rows == 0) {
      setErr(err, errSize, "Day 12 shape definition has no rows.");
      free(lines);
      return 0;
    }
    shapeCount++;
  }

  if (shapeCount == 0) {
    setErr(err, errSize, "Day 12 input defines no shapes.");
    free(lines);
    return 0;
  }

  // Parse regions
  int regions = 0;
  for (; lineIdx < lineCount; ++lineIdx) {
    if (lines[lineIdx].len == 0) {
      continue;
    }
    const LineView *lv = &lines[lineIdx];
    const char *xpos = memchr(lv->ptr, 'x', lv->len);
    const char *colon = memchr(lv->ptr, ':', lv->len);
    if (!xpos || !colon || xpos > colon) {
      setErr(err, errSize,
             "Day 12 region lines must look like 'WxH: counts...'.");
      free(lines);
      return 0;
    }
    unsigned long long w = 0, h = 0;
    const char *ptr = lv->ptr;
    if (!parseUnsigned(&ptr, &w) || ptr != xpos) {
      setErr(err, errSize, "Day 12 region width is invalid.");
      free(lines);
      return 0;
    }
    ptr++; // skip 'x'
    if (!parseUnsigned(&ptr, &h) || ptr != colon) {
      setErr(err, errSize, "Day 12 region height is invalid.");
      free(lines);
      return 0;
    }
    ptr++; // skip ':'
    int counts = 0;
    while (ptr < lv->ptr + lv->len) {
      while (ptr < lv->ptr + lv->len &&
             (*ptr == ' ' || *ptr == '\t')) {
        ptr++;
      }
      if (ptr >= lv->ptr + lv->len) {
        break;
      }
      unsigned long long c;
      if (!parseUnsigned(&ptr, &c)) {
        setErr(err, errSize,
               "Day 12 region counts must be integers separated by spaces.");
        free(lines);
        return 0;
      }
      counts++;
    }
    if (counts == 0) {
      setErr(err, errSize,
             "Day 12 region line has no shape counts listed.");
      free(lines);
      return 0;
    }
    if (counts != shapeCount) {
      setErr(err, errSize,
             "Day 12 region count length must match number of shapes.");
      free(lines);
      return 0;
    }
    regions++;
  }

  free(lines);
  if (regions == 0) {
    setErr(err, errSize, "Day 12 input lists no regions.");
    return 0;
  }
  return 1;
}

int validateInputForDay(unsigned int day, const char *data, size_t size,
                        char *errBuf, size_t errBufSize) {
  switch (day) {
  case 1:
    return validateDay1(data, size, errBuf, errBufSize);
  case 2:
    return validateDay2(data, size, errBuf, errBufSize);
  case 3:
    return validateDay3(data, size, errBuf, errBufSize);
  case 4:
    return validateDay4(data, size, errBuf, errBufSize);
  case 5:
    return validateDay5(data, size, errBuf, errBufSize);
  case 6:
    return validateDay6(data, size, errBuf, errBufSize);
  case 7:
    return validateDay7(data, size, errBuf, errBufSize);
  case 8:
    return validateDay8(data, size, errBuf, errBufSize);
  case 9:
    return validateDay9(data, size, errBuf, errBufSize);
  case 10:
    return validateDay10(data, size, errBuf, errBufSize);
  case 11:
    return validateDay11(data, size, errBuf, errBufSize);
  case 12:
    return validateDay12(data, size, errBuf, errBufSize);
  default:
    setErr(errBuf, errBufSize, "Unsupported day for validation.");
    return 0;
  }
}
