#include "drawing.h"

void draw_square(int x, int y, int scale, unsigned short** buffer, int color) {
    int color = 0xFFFF;
    if (color == 1) {
        color = 0xF800;
    } else if (color == 2) {
        color = 0x001F ;
    }
    

    for (int i = 0; i < scale; i++) {
        for (int j = 0; j < scale; j++) {
            buffer[x+i][y+j] = color;
        }
    }
}
void draw_vert_line(int start_x, int start_y, int length, int scale, unsigned short** buffer, int color) {
    int repeats = length / scale;

    for (int i = 0; i < repeats; i += scale) {
        draw_square(start_x, start_y + i, scale, buffer, color);
    }
    int last_square_y = length - scale;
    draw_square(start_x, last_square_y, scale, buffer, color);

}
void draw_horziontal_line(int start_x, int start_y, int length, int scale, unsigned short** buffer, int color) {
    int repeats = length / scale;

    for (int i = 0; i < repeats; i += scale) {
        draw_square(start_x + i, start_y, scale, buffer, color);
    }
    int last_square_x = length - scale;
    draw_square(last_square_x, start_y, scale, buffer, color);
}