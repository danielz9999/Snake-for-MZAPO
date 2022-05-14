#include "snake_head.h"
#include <stdio.h>
#include <stdbool.h>

enum Directions {UP=1, RIGHT=2, DOWN=3, LEFT=4};
enum MovementVariables {FRUIT = 10, HEAD = 6};

//Moves snake along the playspace, return value determines whether a fruit has been eaten or not
bool movement(char** playSpace, unsigned char* parlcd_mem_base, snake_head *snakeHead,
        snake_head *snakeTail, int direction) {
            
    bool ateFruit = false;        
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

    }
    //If snake is not growing, move tail up a position
    direction = playSpace[snakeTail->x][snakeTail->y];
    playSpace[snakeTail->x][snakeTail->y] = 0;
    if(playSpace[snakeHead->x][snakeHead->y] != FRUIT) {
        switch (direction) {
            case UP:
                snakeTail->y -= 1;
            case RIGHT:
                snakeTail->x += 1;
            case DOWN:
                snakeTail->y += 1;
            case LEFT:
                snakeTail->x -= 1;
            default:
                fprintf(stderr, "Wrong direction in playSpace.");
        }
        ateFruit = true;
    }
    //if snake is growing tail is not moved
    //write snake head into playSpace
    playSpace[snakeHead->x][snakeHead->y] = HEAD;
    return ateFruit;
}