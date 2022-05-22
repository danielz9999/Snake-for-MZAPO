#include <time.h>

#ifndef MAINMENU_H
#define MAINMENU_H
//Draws the first screen of the game, with text saying "PLAYERS?" and the options "1" and "2"
void draw_first_screen(unsigned char* parlcd_mem_base);
//Calls draw first screen and the lets the player choose between 1 and 2 players
bool mainmenu(unsigned char* mem_base, unsigned char* parlcd_mem_base, struct timespec* clock);
#endif  /*MAINMENU_H*/