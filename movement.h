#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdbool.h>

enum Directions {UP=1, RIGHT=2, DOWN=3, LEFT=4};

bool movement(unsigned char** playSpace, bool* has_fruit_been_eaten, snake_head *snakeHead, snake_head *snakeTail, int direction);

#endif  /*MOVEMENT_H*/