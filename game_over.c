#include <time.h>
#include <stdbool.h>
#include <mzapo_regs.h>
#include <stdlib.h>

#include "game_over.h"
#include "drawing.h"


int SNAKE_SIZE = 5;

void game_over(unsigned char* mem_base, unsigned char* parlcd_mem_base, bool two_players, bool red_loss, bool blue_loss,
               unsigned char** playspace, snake_head* tail_one, snake_head* tail_two, snake_head* head_one, snake_head* head_two) {
    struct timespec end_clock;
    end_clock.tv_nsec = 300 * 1000 * 1000;
    end_clock.tv_sec = 0;

    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0xFFFFFFFF;
    if ((red_loss && blue_loss) || !two_players) {
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00FF0000;
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00FF0000;
      snake_death_animation(tail_one, head_one, playspace, &end_clock, parlcd_mem_base);
      if (two_players) {
          snake_death_animation(tail_two, head_two, playspace, &end_clock, parlcd_mem_base);
      }
    } else if(two_players && red_loss) {
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00FF0000;
       snake_death_animation(tail_one, head_one, playspace, &end_clock, parlcd_mem_base);
    } else if (blue_loss) {
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00FF0000;
      snake_death_animation(tail_two, head_two, playspace, &end_clock, parlcd_mem_base);
    }
    
    for (int i = 0; i < 10; i++) {
      clock_nanosleep(CLOCK_MONOTONIC, 0, &end_clock, NULL);
    }
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00000000;
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00000000;
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x00000000;
    for (int i = 0; i < 480; i++) {
        for (int j = 0; j <320; j++) {
            playspace[i][j] = 0;
        }
    }
    draw(playspace, parlcd_mem_base);
    exit(0);
}

void snake_death_animation(snake_head* tail, snake_head* head, unsigned char** playspace, struct timespec* clock,
                            unsigned char* parlcd_mem_base) {
    int even_offset = 0;
    if ((SNAKE_SIZE % 2) == 0) {
        even_offset = 1;
    }

    while (!(tail->x == head->x && tail->y == head->y)) {
        int next_dir = playspace[tail->x][tail->y];
        for (int i = 0; i < SNAKE_SIZE; i++) {
            for (int j = 0; j < SNAKE_SIZE; j++) {
                playspace[tail->x - (SNAKE_SIZE/2 - even_offset) + i][tail->y - (SNAKE_SIZE/2 - even_offset) + j] = 0;
            }
        }
        switch (next_dir) {
            case 1:
                tail->y -= SNAKE_SIZE;
                break;
                
            case 2:
                tail->x += SNAKE_SIZE;
                break;
                
            case 3:
                tail->y += SNAKE_SIZE;
                break;
            case 4:
                tail->x -= SNAKE_SIZE;
                break;
            default:
                break;

        }
        draw(playspace, parlcd_mem_base);
        clock_nanosleep(CLOCK_MONOTONIC, 0, clock, NULL);
        if (tail->x < 0 || tail->x >= 480 || tail->y < 0 || tail->y >= 320) {
            break;
        }
    }
}