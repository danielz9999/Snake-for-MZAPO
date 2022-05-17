#include "snake_head.h"
#include <stdio.h>
#include <stdbool.h>

enum Directions {UP=1, RIGHT=2, DOWN=3, LEFT=4};
enum MovementVariables {FRUIT = 10, HEAD = 6};

//Moves snake along the playspace, return value determines whether a fruit has been eaten or not
bool movement(unsigned char** playSpace, bool* has_fruit_been_eaten, snake_head *snakeHead,
        snake_head *snakeTail, int direction) {
            
        
    //move the head
    playSpace[snakeHead->x][snakeHead->y] = direction;
    switch (direction) {
        case 1:
            snakeHead->y -= 1;
            break;
            
        case 2:
            snakeHead->x += 1;
            break;
            
        case 3:
            snakeHead->y += 1;
            break;
        case 4:
            snakeHead->x -= 1;
            break;
        default:
            fprintf(stderr, "Wrong direction given. dir: %d, up %d, right, %d, down %d, left %d \n", direction, UP, RIGHT, DOWN, LEFT);

    }
    //Out of bounds check
    if (snakeHead->x < 0 || snakeHead->x > 480 || snakeHead->y < 0 || snakeHead->y > 320) {
        return true;
    }
    //Colision check
    if (playSpace[snakeHead->x][snakeHead->y] != 0 && playSpace[snakeHead->x][snakeHead->y] != 10) {
        return true;
    }
    //If both checks passed, move snake head
    playSpace[snakeHead->x][snakeHead->y] = direction;
    
    //If snake is not growing, move tail up a position
    direction = playSpace[snakeTail->x][snakeTail->y];
    playSpace[snakeTail->x][snakeTail->y] = 0;
    if(playSpace[snakeHead->x][snakeHead->y] != FRUIT) {
        switch (direction) {
            case UP:
                snakeTail->y -= 1;
            	break;
            case RIGHT:
                snakeTail->x += 1;
            	break;
            case DOWN:
                snakeTail->y += 1;
            	break;
            case LEFT:
                snakeTail->x -= 1;
            	break;
            default:
                fprintf(stderr, "Wrong tail direction in playSpace. dir %d \n", direction);
        	break;
        }
        *(has_fruit_been_eaten) = false;
    } else {
        *(has_fruit_been_eaten) = true;
    }
    
    //if snake is growing tail is not moved
    //write snake head into playSpace
    
    return false;
}
