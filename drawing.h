/*
Basic drawing functions to the frame buffer
The color parameter decides which color to use,
0 - WHITE
1 - RED
2 - BLUE
3 - GREEN
*/
#ifndef DRAWING_H
#define DRAWING_H

#include <stdint.h>
//Draws a square of white, size depends on scal
void draw_square(int x, int y, int scale, unsigned short** buffer, unsigned short color);

//Draws a vertical line using the draw square function
void draw_vert_line(int start_x, int start_y, int length, int scale, unsigned short** buffer, unsigned short color);

//Draws a horizontal line using the draw square function
void draw_horziontal_line(int start_x, int start_y, int length, int scale, unsigned short** buffer, unsigned short color);

//Functions for decoding the logical playspace into something the LCD display can parse

//graphic_decode turns a singel pixel from the logic playspace into a color that it returns
unsigned short graphic_decode(char input);
//Calls graphic decode on every pixel in the playspace, sends them to the LCD display
void draw(unsigned char** playspace, unsigned char* parlcd_mem_base);

#endif  /*DRAWING_H*/