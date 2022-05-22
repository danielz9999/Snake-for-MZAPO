#include "snake_head.h"
#include <stdbool.h>
#ifndef GAME_OVER_H
#define GAME_OVER_H

//Removes the snake from the display piece by piece starting at tail
void snake_death_animation(snake_head* tail, snake_head* head, unsigned char** playspace, struct timespec* clock,
                            unsigned char* parlcd_mem_base);
//Handles game over, flashes red RGB diodes for losing player, turns on all LED lights then exits after a short while
//Calls snake_death_animation
void game_over(unsigned char* mem_base, unsigned char* parlcd_mem_base, bool two_players, bool red_loss, bool blue_loss,
               unsigned char** playspace, snake_head* tail_one, snake_head* tail_two, snake_head* head_one, snake_head* head_two);


#endif  /*GAME_OVER_H*/