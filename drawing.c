#include "drawing.h"

void draw_square(int x, int y, int scale, unsigned short** buffer, unsigned short color) {
    unsigned short drawing_color = 0xFFFF;
    if (color == 1) {
        drawing_color = 0xF800;
    } else if (color == 2) {
        drawing_color = 0x001F ;
    } else if (color == 3) {
        drawing_color = 0x07E0;
    }
    

    for (int i = 0; i < scale; i++) {
        for (int j = 0; j < scale; j++) {
            buffer[x+i][y+j] = drawing_color;
        }
    }
}
void draw_vert_line(int start_x, int start_y, int length, int scale, unsigned short** buffer, unsigned short color) {
    int repeats = (int) length / scale;

    for (int i = 0; i < repeats; i++) {
        draw_square(start_x, start_y + i*scale, scale, buffer, color);
    }
    int last_square_y = start_y + length - scale;
    draw_square(start_x, last_square_y, scale, buffer, color);

}
void draw_horziontal_line(int start_x, int start_y, int length, int scale, unsigned short** buffer, unsigned short color) {
    int repeats = (int) length / scale;

    for (int i = 0; i < repeats; i++) {
        draw_square(start_x + i*scale, start_y, scale, buffer, color);
    }
    int last_square_x = start_x + length - scale;
    draw_square(last_square_x, start_y, scale, buffer, color);
}
