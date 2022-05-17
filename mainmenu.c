#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "drawing.h"
#include "mzapo_regs.h"
#include "mainmenu.h"



bool mainmenu(unsigned char* mem_base, unsigned char* parlcd_mem_base) {
 draw_first_screen(parlcd_mem_base);
 unsigned int knob_values = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
 unsigned int red_knob = (knob_values>>16) & 0xFF;
 unsigned int blue_knob = knob_values & 0xFF;
 int red_button = 0;
 int blue_button = 0;
 while (1) {
    knob_values = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    red_button =  (knob_values>>26) & 1;
    blue_button = (knob_values>>24) & 1;
    if (red_button) {
        return false;
    } else if ( blue_button) {
        return true;
    }
    int new_red_knob = (knob_values>>16) & 0xFF;
    int new_blue_knob = knob_values & 0xFF;
    if (abs(red_knob - new_red_knob) > 1) {
        return false;
    } else if (abs(blue_knob - new_blue_knob) > 1) {
        return true;
    }
    red_knob = new_red_knob;
    blue_knob = new_blue_knob;
 }
}

void draw_first_screen(unsigned char* parlcd_mem_base) {
    //Init buffer
    unsigned short** frame_buffer = calloc(480, sizeof(short*));
    for (int i = 0; i < 480; i++) {
        frame_buffer[i] = calloc(320, sizeof(short));
    }
    //Draw "PLAYERS?" to the buffer
    int height = 50;
    int pixel_scale = 6;
    int start_y = 14;
    int start_x = 132;

    //P
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 0);
    start_x += 6;
    draw_horziontal_line(start_x, start_y, 16, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, 29, 16, pixel_scale, frame_buffer, 0);
    draw_vert_line(start_x+16-pixel_scale, start_y+pixel_scale, 9, pixel_scale, frame_buffer, 0);

    //L
    start_x += (16+6);
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + height - pixel_scale, 21, pixel_scale, frame_buffer, 0);
    start_x += (21+pixel_scale);

    //A
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 0);
    start_x += pixel_scale;
    draw_horziontal_line(start_x, start_y, 12, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + 16, 12, pixel_scale, frame_buffer, 0);
    start_x += 12;
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 0);

    //Y
    start_x += pixel_scale*2;
    draw_vert_line(start_x, start_y, 20, pixel_scale, frame_buffer, 0);
    start_x += pixel_scale;
    draw_horziontal_line(start_x, start_y+20-pixel_scale, 10, pixel_scale, frame_buffer, 0);
    start_x += 2;
    draw_vert_line(start_x, start_y + 20, 30, pixel_scale, frame_buffer, 0);
    start_x += pixel_scale + 2;
    draw_vert_line(start_x, start_y, 20, pixel_scale, frame_buffer, 0);

    //E
    start_x += pixel_scale*2;
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 0);
    start_x += pixel_scale;
    draw_horziontal_line(start_x, start_y, 16, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + 16, 16, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + height - pixel_scale, 16, pixel_scale, frame_buffer, 0);

    //R
    start_x += 16 + pixel_scale;
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 0);
    start_x += 6;
    draw_horziontal_line(start_x, start_y, 16, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, 29, 16, pixel_scale, frame_buffer, 0);
    draw_vert_line(start_x+16-pixel_scale, start_y+pixel_scale, 9, pixel_scale, frame_buffer, 0);
    start_x = 275;
    start_y = 33;
    for (; start_y <= 57; start_y++) {
        if ((start_y % 2) == 0) {
            start_x++;
        }
        draw_square(start_x, start_y, pixel_scale, frame_buffer, 0);
    }
    //S
    start_x = 300;
    start_y = 14;
    draw_vert_line(start_x, start_y, 27, pixel_scale, frame_buffer, 0);
    draw_vert_line(start_x, 56, 8, pixel_scale, frame_buffer, 0);
    start_x += pixel_scale;
    draw_horziontal_line(start_x, start_y, 7, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + 21, 7, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + 44, 7, pixel_scale, frame_buffer, 0);
    start_x += 7;
    draw_vert_line(start_x, start_y, 8, pixel_scale, frame_buffer, 0);
    draw_vert_line(start_x, start_y + 21, 29, pixel_scale, frame_buffer, 0);
    //?
    start_x += pixel_scale*2;

    draw_vert_line(start_x, start_y, 11, pixel_scale, frame_buffer, 0);
    draw_vert_line(start_x, start_y + 11 + pixel_scale, 25, pixel_scale, frame_buffer, 0);
    start_x += pixel_scale;
    draw_horziontal_line(start_x, start_y, 7, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + 11 + pixel_scale, 7, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + 11 + 25, 7, pixel_scale, frame_buffer, 0);
    start_x += 7;
    draw_vert_line(start_x, start_y, 23, pixel_scale, frame_buffer, 0);
    draw_vert_line(start_x, start_y + 30, 12, pixel_scale, frame_buffer, 0);
    start_x -= (7);
    draw_square(start_x, start_y + 44, pixel_scale, frame_buffer, 0);

    //Draw a red 1 in the left bottom of the screen
    start_x = 93;
    start_y = 211;
    draw_vert_line(start_x, start_y, 68, pixel_scale, frame_buffer, 1);
    for (int i = 0; i < 18; i++) {
        start_x--;
        start_y++;
        draw_square(start_x, start_y, pixel_scale, frame_buffer, 1);
    }
    //Draw a blue 2 in the right bottom of the screen
    start_x = 369;
    start_y = 211;
    draw_vert_line(start_x, start_y, 17, pixel_scale, frame_buffer, 2);
    draw_vert_line(start_x, start_y + 34, 40, pixel_scale, frame_buffer, 2);
    start_x += pixel_scale;
    draw_horziontal_line(start_x, start_y, 23, pixel_scale, frame_buffer, 2);
    draw_horziontal_line(start_x, start_y + 34, 23, pixel_scale, frame_buffer, 2);
    draw_horziontal_line(start_x, start_y + 34 + pixel_scale + 28, 23 + pixel_scale, pixel_scale, frame_buffer, 2);
    start_x += 23;
    draw_vert_line(start_x, start_y, 40, pixel_scale, frame_buffer, 2);

    //Sends frame buffer to LCD 
    *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_CMD_o) = 0x2c;
    for (int i = 0; i < 320; i++) {
        for (int j = 0; j < 480; j++) {
            *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = frame_buffer[j][i];
        }
    }

}
