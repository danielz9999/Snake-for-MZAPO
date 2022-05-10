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

enum Directions {UP=1, RIGHT=2, DOWN=3, LEFT=4};
unsigned short graphicDecode(char input);
void draw(char** playspace, unsigned char* parlcd_mem_base);



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
  snake_head head_one;
  head_one.x = 240;
  head_one.y = 160;
  unsigned char** playspace = calloc(320, sizeof(char*));
  for (int i = 0; i < 320; i++) {
    playspace[i] = calloc(480, sizeof(char));
  }
  for (int i = 0; i < 5; i++) {
    playspace[head_one.x][head_one.y+i] = LEFT;
  }
  

  //mapping
  unsigned char* mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  unsigned char* parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

  if (mem_base == NULL || parlcd_mem_base == NULL) {
    printf(stderr, "Error mapping memmory, exiting");
    exit(1);
    
  }


  //Intitalise the LCD display
  parlcd_hx8357_init(parlcd_mem_base);

  draw(playspace, parlcd_mem_base);
  
  exit(1);
  while (1)
  {
    
  }
  

  /* Release the lock */
  serialize_unlock();

  return 0;
}
//Decodes the logic in the playspace array into values which can be passed to the LCD display
unsigned short graphicDecode(char input) {
  if (input > 0 && input < 10) {
    return 0xFFFF;
  } else if (input == 10) {
    return 0xF800;
  } else {
    return 0x0;
  }
  
}
void draw(char** playspace, unsigned char* parlcd_mem_base) {
  *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_CMD_o) = 0x2c;
  for (int i = 0; i < 320; i++) {
    for (int j = 0; j < 480; j++) {
        *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = graphicDecode(playspace[i][j]);
    }
  }
  //*
}