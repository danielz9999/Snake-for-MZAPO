#ifndef SPEED_MENU_H
#define SPEED_MENU_H


//A menu where the player chooses game speed, begins by calling draw_screen
//0 -> 0.1x speed
//1 -> 1x speed (normal)
//2 -> 10x speed
int speed_menu(unsigned char* mem_base, unsigned char* parlcd_mem_base, struct timespec* clock);
//Draws the speed menu screen
void draw_screen(unsigned char* parlcd_mem_base);

#endif  /*SPEED_MENU_H*/