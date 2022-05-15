/*
Basic drawing functions to the frame buffer
The color parameter decides which color to use,
0 - WHITE
1 - RED
2 - BLUE
*/
#ifndef DRAWING_H
#define DRAWING_H

#include <stdint.h>
//Draws a square of white, size depends on scal
void draw_square(int x, int y, int scale, unsigned short** buffer, int color);

//Draws a vertical line using the draw square function
void draw_vert_line(int start_x, int start_y, int length, int scale, unsigned short** buffer, int color);

//Draws a horizontal line using the draw square function
void draw_horziontal_line(int start_x, int start_y, int length, int scale, unsigned short** buffer, int color);

#endif  /*DRAWING_H*/