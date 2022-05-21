/*******************************************************************
  Project main function template for MicroZed based MZ_APO board
  designed by Petr Porazil at PiKRON

  change_me.c      - main file

  include your name there and license for distribution.

  Remove next text: This line should not appear in submitted
  work and project name should be change to match real application.
  If this text is there I want 10 points subtracted from final
  evaluation.

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "serialize_lock.h"
#include "snake_head.h"
#include "movement.h"
#include "mainmenu.h"
#include "fruit_generation.h"
#include "drawing.h"
#include "knob_parsing.h"

#define FRUIT 10
#define SNAKE_SIZE 5

//enum Directions {UP=1, RIGHT=2, DOWN=3, LEFT=4};
unsigned short graphicDecode(char input);
void draw(unsigned char** playspace, unsigned char* parlcd_mem_base);
void fruit_get(int* score, unsigned char* mem_base, struct timespec* clock);
void game_over(unsigned char* mem_base, struct timespec* clock);
void timer_decrement(struct timespec* clock);


int main(int argc, char *argv[]) {

  /* Serialize execution of applications */

  /* Try to acquire lock the first */
  if (serialize_lock(1) <= 0) {
    printf("System is occupied\n");

    if (1) {
      printf("Waitting\n");
      /* Wait till application holding lock releases it or exits */
      serialize_lock(0);
    }
  }

  //Initializing variables
  int score_counter = 1;
  bool has_fruit_been_eaten;
  bool is_game_over = false;
  int even_offset = 0;
  bool has_red_turned = false;
  if ((SNAKE_SIZE % 2) == 0) {
    even_offset = 1;
  }
  

  unsigned char** playspace = calloc(480, sizeof(char*));
  for (int i = 0; i < 480; i++) {
    playspace[i] = calloc(320, sizeof(char));
  }

 

  //Initial assiging to the logic board

  snake_head head_one;
  snake_head tail_one;

  snake_head fruit_coordinates;
  fruit_coordinates.x = 240;
  fruit_coordinates.y = 160;
  generate_fruit(playspace, &fruit_coordinates.x, &fruit_coordinates.y, SNAKE_SIZE);

  struct timespec clock_spec;
  clock_spec.tv_sec = 0;
  clock_spec.tv_nsec = 15 * 1000000;
  

  //mapping
  unsigned char* mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  unsigned char* parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

  if (mem_base == NULL || parlcd_mem_base == NULL) {
    printf(stderr, "Error mapping memmory, exiting");
    exit(1);
    
  }
  //Initialise knob values
  unsigned int knob_values = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
  unsigned int red_knob = (knob_values>>16) & 0xFF;
  //Initialise the LCD display
  parlcd_hx8357_init(parlcd_mem_base);

  bool two_players = mainmenu(mem_base, parlcd_mem_base);
  if (two_players) {
      head_one.x = 160;
      tail_one.x = 160;
  } else {
    head_one.x = 240;
    tail_one.x = 240;
  }
  head_one.y = 160;
  tail_one.y = 160 + 5 * (SNAKE_SIZE + even_offset - 1);     

  int start_x = head_one.x - (SNAKE_SIZE/2) + even_offset;
  int start_y = head_one.y - (SNAKE_SIZE/2) + even_offset;
  for (int i = 0; i < 5; i++) {
    for (int x_offset = 0; x_offset < SNAKE_SIZE; x_offset++) {
      for (int y_offset = 0 ; y_offset < SNAKE_SIZE; y_offset++) {
        playspace[start_x + x_offset][start_y + y_offset + i*(SNAKE_SIZE)] = UP;
      }
    }
  }
  draw(playspace, parlcd_mem_base);
  char current_direction;
  //Main game loop
  while (1)
  {
    current_direction = playspace[head_one.x][head_one.y];
    int new_knob_values = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    int new_red_knob = (new_knob_values>>16) & 0xFF;
    if (!has_red_turned) {
      current_direction = parse_knob(new_red_knob, red_knob, &has_red_turned, current_direction);
    } else {
      has_red_turned = false;
    }
    red_knob = new_red_knob;


    is_game_over = movement(playspace, &has_fruit_been_eaten, &head_one, &tail_one, current_direction, SNAKE_SIZE);
    if (is_game_over) {
      game_over(mem_base, &clock_spec);
    }
    if (has_fruit_been_eaten) {
      fruit_get(&score_counter, mem_base, &clock_spec);
      generate_fruit(playspace, &fruit_coordinates.x, &fruit_coordinates.y, SNAKE_SIZE);
    }

    //display changes
    draw(playspace, parlcd_mem_base);

    //pause before next cycle
    clock_nanosleep(CLOCK_MONOTONIC, 0, &clock_spec, NULL);
    //Resets RGB lights one tick after eating fruit
    if (!has_fruit_been_eaten) {
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00000000;
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00000000; 
    }

  }

  /* Release the lock */
  serialize_unlock();

  return 0;
}

//Triggers when a snake eats a fruit
void fruit_get(int* score, unsigned char* mem_base, struct timespec* clock) {
  //The game progessively gets faster when fruit is picked up
  timer_decrement(clock);

  //If score reached max, celebrate, then reset score
  if (*(score) >= 0xFFFFFFFF) {
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00FFFF00;
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00FFFF00; 

    for (int i = 0; i < 100; i++) {
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0xAAAAAAAA;
      clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x55555555;
      clock_nanosleep(CLOCK_MONOTONIC, 0 , clock, NULL);
    }
    *(score) = 1;
  } else {
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = *(score);
    int temp = *(score);
    *(score)<<=1;
    *(score) += temp;
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x0000FF00;
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x0000FF00; 
    }
}
//Decodes the logic in the playspace array into values which can be passed to the LCD display
unsigned short graphicDecode(char input) {
  if (input > 0 && input != FRUIT) {
    return 0xFFFF;
  } else if (input == FRUIT) {
    return 0xF800;
  } else {
    return 0x0;
  }
  
}
//Displays the current state of the game to the LCD display
//Loops through x coordinate first, since
void draw(unsigned char** playspace, unsigned char* parlcd_mem_base) {
  *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_CMD_o) = 0x2c;
   for (int i = 0; i < 320; i++) {
    for (int j = 0; j < 480; j++) {
        *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = graphicDecode(playspace[j][i]);
    }
  }
  
}
void game_over(unsigned char* mem_base, struct timespec* clock) {
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00FF0000;
    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00FF0000;

    *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0xFFFFFFFF;
    clock->tv_nsec = 300 * 1000 * 1000;
    for (int i = 0; i < 10; i++) {
      clock_nanosleep(CLOCK_MONOTONIC, 0, clock, NULL);
    }
    exit(0);
}
void timer_decrement(struct timespec* clock) {
  clock->tv_nsec = clock->tv_nsec - 30000;
}


