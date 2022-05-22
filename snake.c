/*******************************************************************
  snake
  Application developed by Daniel Žampach and Petr Stanke
  for the MicroZed based MZ_APO board during the 2022 summer semester.

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
#include "snake_head.h"
#include "movement.h"
#include "mainmenu.h"
#include "fruit.h"
#include "drawing.h"
#include "knob_parsing.h"
#include "speed_menu.h"
#include "game_over.h"

#define FRUIT 10
#define SNAKE_SIZE 5

int main(int argc, char *argv[]) {
  //Initializing variables
  unsigned int red_score = 0;
  unsigned int blue_score = 0;
  int even_offset = 0;
  int rgb_duration = 0;
  char red_direction;
  char blue_direction;
  bool red_ate_fruit = false;
  bool blue_ate_fruit = false;
  bool red_game_over = false;
  bool blue_game_over = false;
  bool has_red_turned = false;
  bool has_blue_turned = false;
  
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

  snake_head head_two;
  snake_head tail_two;

  snake_head fruit_coordinates;
  fruit_coordinates.x = 240;
  fruit_coordinates.y = 160;
  srand(time(NULL));
  generate_fruit(playspace, &fruit_coordinates.x, &fruit_coordinates.y, SNAKE_SIZE);
  
  struct timespec clock_spec;
  clock_spec.tv_sec = 0;
  clock_spec.tv_nsec = 10 * 1000000;

  //mapping
  unsigned char* mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  unsigned char* parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
  if (mem_base == NULL || parlcd_mem_base == NULL) {
    printf(stderr, "Error mapping memmory, exiting");
    exit(1);
  }
  //Reset LED line and RGB diodes
  *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00000000;
  *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00000000;
  *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = 0x00000000;
  //Initialise knob values
  unsigned int knob_values = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
  unsigned int red_knob = (knob_values>>16) & 0xFF;
  unsigned int blue_knob = knob_values & 0xFF;
  //Initialise the LCD display
  parlcd_hx8357_init(parlcd_mem_base);

  bool two_players = mainmenu(mem_base, parlcd_mem_base, &clock_spec);
  int speed = speed_menu(mem_base, parlcd_mem_base, &clock_spec);
  if (speed == 0) {
    clock_spec.tv_nsec *= 10;
  } else if (speed == 2) {
    clock_spec.tv_nsec /= 10;
  }
  if (two_players) {
      head_one.x = 160;
      tail_one.x = 160;

      head_two.x = 320;
      head_two.y = 160;
      tail_two.x = 320;
      tail_two.y = 160 + 5 * (SNAKE_SIZE + even_offset - 1);     
  } else {
    head_one.x = 240;
    tail_one.x = 240;
  }
  head_one.y = 160;
  tail_one.y = 160 + 5 * (SNAKE_SIZE + even_offset - 1);     

  int start_x = head_one.x - (SNAKE_SIZE/2) + even_offset;
  int start_y = head_one.y - (SNAKE_SIZE/2) + even_offset;
  //Draw initial setup of game to LCD display
  for (int i = 0; i < 5; i++) {
    for (int x_offset = 0; x_offset < SNAKE_SIZE; x_offset++) {
      for (int y_offset = 0 ; y_offset < SNAKE_SIZE; y_offset++) {
        playspace[start_x + x_offset][start_y + y_offset + i*(SNAKE_SIZE)] = UP;
        if (two_players) {
          playspace[start_x + x_offset + 160][start_y + y_offset + i*(SNAKE_SIZE)] = UP;
        }
      }
    }
  }
  draw(playspace, parlcd_mem_base);
  //Main game loop
  while (1)
  {
    red_direction = playspace[head_one.x][head_one.y];
    if (two_players) {
      blue_direction = playspace[head_two.x][head_two.y];
    }
    unsigned int new_knob_values = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    unsigned int new_red_knob = (new_knob_values>>16) & 0xFF;
    unsigned int new_blue_knob = new_knob_values & 0xFF; 
    if (!has_red_turned) {
      red_direction = parse_knob(new_red_knob, red_knob, &has_red_turned, red_direction);
    } else {
      has_red_turned = false;
    }
    if (!has_blue_turned && two_players) {
      blue_direction = parse_knob(new_blue_knob, blue_knob, &has_blue_turned, blue_direction);
    } else {
      has_blue_turned = false;
    }
    red_knob = new_red_knob;
    blue_knob = new_blue_knob;
    //Move snakes
    red_game_over = movement(playspace, &red_ate_fruit, &head_one, &tail_one, red_direction, SNAKE_SIZE);
    if (two_players) {
      blue_game_over = movement(playspace, &blue_ate_fruit, &head_two, &tail_two, blue_direction, SNAKE_SIZE);
    }
    //Trigger game over
    if (red_game_over || blue_game_over) {
      game_over(mem_base, parlcd_mem_base, two_players, red_game_over, blue_game_over, playspace,
               &tail_one, &tail_two, &head_one, &head_two);
    }
    //Increase score on fruit pickup
    if (red_ate_fruit) {
      fruit_get(&red_score, &blue_score, mem_base ,&clock_spec, two_players, 1);
      generate_fruit(playspace, &fruit_coordinates.x, &fruit_coordinates.y, SNAKE_SIZE);
      rgb_duration += 5;
    }
    if (blue_ate_fruit) {
      fruit_get(&red_score, &blue_score, mem_base ,&clock_spec, two_players, 2);
      generate_fruit(playspace, &fruit_coordinates.x, &fruit_coordinates.y, SNAKE_SIZE);
      rgb_duration += 5;
    }
    //display changes
    draw(playspace, parlcd_mem_base);
    //pause before next cycle
    clock_nanosleep(CLOCK_MONOTONIC, 0, &clock_spec, NULL);
    //Resets RGB lights after their duration runs out
    if (rgb_duration == 0) {
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB1_o) = 0x00000000;
      *(volatile uint32_t*)(mem_base + SPILED_REG_LED_RGB2_o) = 0x00000000; 
    } else {
      rgb_duration--;
    }
  }
  return 0;
}
