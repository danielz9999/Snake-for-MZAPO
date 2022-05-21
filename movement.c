#include "snake_head.h"
#include <stdio.h>
#include <stdbool.h>

enum Directions {UP=1, RIGHT=2, DOWN=3, LEFT=4};
enum MovementVariables {FRUIT = 10, HEAD = 6};

//Moves snake along the playspace, return value determines whether a fruit has been eaten or not
bool movement(unsigned char** playSpace, bool* has_fruit_been_eaten, snake_head *snakeHead,
        snake_head *snakeTail, int direction, int snake_size) {

    int even_offset = 0;
    if (snake_size % 2 == 0) {
        even_offset = 1;
    }
    *(has_fruit_been_eaten) = false;
    //move the head
    playSpace[snakeHead->x][snakeHead->y] = direction;
    switch (direction) {
        case 1:
            snakeHead->y -= snake_size;
            break;
            
        case 2:
            snakeHead->x += snake_size;
            break;
            
        case 3:
            snakeHead->y += snake_size;
            break;
        case 4:
            snakeHead->x -= snake_size;
            break;
        default:
            fprintf(stderr, "Wrong direction given. dir: %d, up %d, right, %d, down %d, left %d \n", direction, UP, RIGHT, DOWN, LEFT);

    }
    int upper_left_corner_x = snakeHead->x - (snake_size/2 - even_offset);
    int upper_left_corner_y = snakeHead->y - (snake_size/2 - even_offset);
    int lower_right_corner_x = snakeHead->x + (snake_size/2);
    int lower_right_corner_y = snakeHead->y + (snake_size/2);
    //Out of bounds check
    if (upper_left_corner_x < 0 || lower_right_corner_x > 480 || upper_left_corner_y < 0 || lower_right_corner_y > 320) {
        return true;
    }
    //Colision check
    for (int i = 0; i < snake_size; i++) {
        for (int j = 0; j < snake_size; j++) {
            if (playSpace[upper_left_corner_x + i][upper_left_corner_y + j] != 0 &&
                playSpace[upper_left_corner_x + i][upper_left_corner_y + j] != 10) {
                return true;
            } else {
                if (playSpace[upper_left_corner_x + i][upper_left_corner_y + j] == 10) {
                    *(has_fruit_been_eaten) = true;
                }
                //If both checks passed, move snake head
                playSpace[upper_left_corner_x + i][upper_left_corner_y + j] = direction;
            }
        }
    }
   
    //If snake is not growing, move tail up a position
    direction = playSpace[snakeTail->x][snakeTail->y];
    if(!*(has_fruit_been_eaten)) {

        for (int i = 0; i < snake_size; i++) {
            for (int j = 0; j < snake_size; j++) {
                playSpace[snakeTail->x - (snake_size/2 - even_offset) + i][snakeTail->y - (snake_size/2 - even_offset) + j] = 0;
            }
        }

        switch (direction) {
            case UP:
                snakeTail->y -= snake_size;
            	break;
            case RIGHT:
                snakeTail->x += snake_size;
            	break;
            case DOWN:
                snakeTail->y += snake_size;
            	break;
            case LEFT:
                snakeTail->x -= snake_size;
            	break;
            default:
                fprintf(stderr, "Wrong tail direction in playSpace. dir %d \n", direction);
        	break;
        }
        
    }
    //if snake is growing tail is not moved
    
    return false;
}
