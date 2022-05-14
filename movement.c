#include "snake_head.h"
#include <stdio.h>
#include <stdbool.h>

enum Directions {UP=1, RIGHT=2, DOWN=3, LEFT=4};
enum MovementVariables {FRUIT = 10, HEAD = 6};

bool movement(char** playSpace, unsigned char* parlcd_mem_base, snake_head *snakeHead,
        struct snake_head *snakeTail, int direction) {
    //move the head
    playSpace[snakeHead->x][snakeHead->y] = direction;
    switch (direction) {
        case UP:
            snakeHead->y -= 1;
        case RIGHT:
            snakeHead->x += 1;
        case DOWN:
            snakeHead->y += 1;
        case LEFT:
            snakeHead->x -= 1;
        default:
            fprintf(stderr, "Wrong direction given.");
            return false;
    }
    //snake is not growing
    if(playSpace[snakeHead->x][snakeHead->y] != FRUIT) {
        switch (direction) {
            case UP:
                snakeHead->y -= 1;
            case RIGHT:
                snakeHead->x += 1;
            case DOWN:
                snakeHead->y += 1;
            case LEFT:
                snakeHead->x -= 1;
            default:
                fprintf(stderr, "Wrong direction in playSpace.");
                return false;
        }
    }
    //if snake is growing tail is not moved
    //write snake head into playSpace
    playSpace[snakeHead->x][snakeHead->y] = HEAD;
    return true;
}