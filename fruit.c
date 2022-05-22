#define _POSIX_C_SOURCE 200112L

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "fruit.h"

// guarantee that fruit will not spawn inside of snake is not implemented - depending on snake representation
void generate_fruit(unsigned char **frame_buffer, int* last_fruit_x_coord, int* last_fruit_y_coord, int snake_size) {
    // variables
    int width = 480;
    int height = 320;
    //Erase picked up fruit
    for (int i = 0; i < snake_size*2; i++) {
        for (int j = 0; j < snake_size*2; j++) {
            if (frame_buffer[*(last_fruit_x_coord) - snake_size + i][*(last_fruit_y_coord) - snake_size + j] == 10) {
                frame_buffer[*(last_fruit_x_coord) - snake_size + i][*(last_fruit_y_coord) - snake_size + j] = 0;
            }
        }
    }
    // generate random coords
    int random_x_coord = rand() % (width - 1);
    int random_y_coord = rand() % (height - 1);
    //calculate distance from previous fruit
    double x_distance = abs(*(last_fruit_x_coord) - random_x_coord);
    double y_distance = abs(*(last_fruit_y_coord) - random_y_coord);
    int fruit_distance = sqrt(pow(x_distance, 2) + pow(y_distance, 2));
    //place fruit into buffer
    if(fruit_distance >= 7) {
        if(frame_buffer[random_x_coord][random_y_coord] == 0) {
            frame_buffer[random_x_coord][random_y_coord] = 10;
            random_x_coord = random_x_coord - snake_size / 2;
            random_y_coord = random_y_coord - snake_size / 2;
            for (int i = 0; i < snake_size; i++) {
                for (int j = 0; j < snake_size; j++) {
                    frame_buffer[random_x_coord + i][random_y_coord + j] = 10;
                }
            }
            *(last_fruit_x_coord) = random_x_coord;
            *(last_fruit_y_coord) = random_y_coord;
        } else {
            generate_fruit(frame_buffer, last_fruit_x_coord, last_fruit_y_coord, snake_size);
        }
    } else {
        generate_fruit(frame_buffer, last_fruit_x_coord, last_fruit_y_coord, snake_size);
    }
}

//Triggers when a snake eats a fruit
void fruit_get(unsigned int* red_score, unsigned int* blue_score, unsigned char* mem_base, struct timespec* clock, bool is_two_players, int player) {
  //The game progessively gets faster when fruit is picked up
  timer_decrement(clock);
  //If two player mode is enabled, each player takes one half of the led display and one RGB diode
  if (is_two_players) {
      if (player == 1) {
        if ((*(red_score) & 0x0000FFFF) > 0) {
            clock->tv_nsec *= 10;
            *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00FFFF00;

            for (int i = 0; i < 8; i++) {
                *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x11111111;
                clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
                *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x22222222;
                clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
                *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x44444444;
                clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
                *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x88888888;
                clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
            }
            clock->tv_nsec /= 10;
            *(red_score) = 0x80000000;
            *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = *(red_score);
            *(red_score)>>=1;
            *(red_score) += 0x80000000;
        } else {
            *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = *(red_score);
            *(red_score)>>=1;
            *(red_score) += 0x80000000;
            *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x0000FF00;

            } 
      } else if (player == 2) {
          if (*(blue_score) >= 0x0000FFFF) {
            clock->tv_nsec *= 10;
            *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00FFFF00;

            for (int i = 0; i < 8; i++) {
                *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x11111111;
                clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
                *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x22222222;
                clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
                *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x44444444;
                clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
                *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x88888888;
                clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
            }
            clock->tv_nsec /= 10;
            *(blue_score) = 1;
            *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = *(blue_score);
            *(blue_score) = 3;
        } else {
            *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = *(blue_score);
            *(blue_score)<<=1;
            *(blue_score) += 0x1;
            *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x0000FF00;

            } 
      }
  } else {
    //If score reached max, celebrate, then reset score
    if (*(red_score) >= 0xFFFFFFFF) {
        clock->tv_nsec *= 10;
        *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00FFFF00;
        *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00FFFF00; 

        for (int i = 0; i < 8; i++) {
            *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x11111111;
            clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
            *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x22222222;
            clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
            *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x44444444;
            clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
            *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x88888888;
            clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
        }
        clock->tv_nsec /= 10;
        *(red_score) = 1;
        *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = *(red_score);
    } else {
        *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = *(red_score);
        *(red_score)<<=1;
        *(red_score) += 0x1;
        *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x0000FF00;
        *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x0000FF00; 
        }
  }
}

void timer_decrement(struct timespec* clock) {
  clock->tv_nsec = clock->tv_nsec - 30000;
}

