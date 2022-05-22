#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdbool.h>

enum Directions {UP=1, RIGHT=2, DOWN=3, LEFT=4};

//Moves the passed snake in the given direction
//Checks for collision with itself, other snakes, for out of bounds moves and for fruit pick ups
bool movement(unsigned char** playSpace, bool* has_fruit_been_eaten, 
             snake_head *snakeHead, snake_head *snakeTail, int direction, int snake_size);

#endif  /*MOVEMENT_H*/