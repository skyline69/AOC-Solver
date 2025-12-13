#ifndef TASK_H
#define TASK_H

enum Direction { Right, Left };

void rotateFastPartOne(unsigned char *value, unsigned int amount, int *counter,
                       enum Direction dir);

void rotateFastPartTwo(unsigned char *value, unsigned int amount, int *counter,
                       enum Direction dir);

#endif
