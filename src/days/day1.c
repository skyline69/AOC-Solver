#include "days/day1.h"

void day1RotateFastPartOne(unsigned char *value, unsigned int amount,
                           int *counter, enum Direction dir) {
  int pos = *value;
  if (dir == Right) {
    pos += (int)amount;
    if (pos >= 100) {
      pos %= 100;
    }
  } else {
    pos -= (int)amount;
    if (pos < 0) {
      pos = (pos % 100 + 100) % 100;
    }
  }
  *value = (unsigned char)pos;
  if (pos == 0) {
    (*counter)++;
  }
}

void day1RotateFastPartTwo(unsigned char *value, unsigned int amount,
                           int *counter, enum Direction dir) {
  int start = (int)(*value);

  unsigned int fullCycles = amount / 100;
  unsigned int remainder = amount % 100;

  *counter += (int)fullCycles;

  if (remainder > 0 && start != 0) {
    if (dir == Right) {
      if (remainder >= (unsigned int)(100 - start)) {
        (*counter)++;
      }
    } else {
      if (remainder >= (unsigned int)start) {
        (*counter)++;
      }
    }
  }

  int pos;
  if (dir == Right) {
    pos = (start + (int)amount) % 100;
  } else {
    pos = (start - (int)amount) % 100;
    if (pos < 0) {
      pos += 100;
    }
  }

  *value = (unsigned char)pos;
}
