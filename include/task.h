#ifndef TASK_H
#define TASK_H

enum Direction { Right, Left };

void day1RotateFastPartOne(unsigned char *value, unsigned int amount,
                           int *counter, enum Direction dir);

void day1RotateFastPartTwo(unsigned char *value, unsigned int amount,
                           int *counter, enum Direction dir);

#endif
