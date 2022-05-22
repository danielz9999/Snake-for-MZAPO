#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "speed_menu.h"
#include "drawing.h"
#include "mzapo_regs.h"
//Draws the screen with the  text "SPEED?" and the options "0.1x 1x 10x" and then lets the player choose
//The choice is done in an infinte loop where one knob+button combo corresponds to one choice
//Turning on pressing the knob will choose that option
int speed_menu(unsigned char* mem_base, unsigned char* parlcd_mem_base, struct timespec* clock) {
    draw_screen(parlcd_mem_base);
    //A pause to not autoaccept the same input as was on the mainmenu screen
    for (int i = 0; i < 10; i++) {
        clock_nanosleep(CLOCK_MONOTONIC, 0, clock, NULL);
    }
    unsigned int knob_values = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
    unsigned int red_knob = (knob_values>>16) & 0xFF;
    unsigned int green_knob = (knob_values>>8) & 0xFF;
    unsigned int blue_knob = knob_values & 0xFF;
    int red_button = 0;
    int green_button = 0;
    int blue_button = 0;
    while (1) {
        unsigned int new_knob_values = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
        unsigned int new_red_knob = (new_knob_values>>16) & 0xFF;
        unsigned int new_green_knob = (new_knob_values>>8) & 0xFF;
        unsigned int new_blue_knob = new_knob_values & 0xFF; 
        red_button =  (new_knob_values>>26) & 1;
        green_button = (new_knob_values>>25) & 1;
        blue_button = (new_knob_values>>24) & 1;
        if (red_button) {
            return 0;
        } else if (green_button) {
            return 1;
        } else if (blue_button) {
            return 2;
        }
        if (abs(red_knob - new_red_knob) > 1) {
            return 0;
        } else if (abs(green_knob - new_green_knob) > 1) {
            return 1;
        } else if (abs(blue_knob - new_blue_knob) > 1) {
            return 2;
        }
        red_knob = new_red_knob;
        green_knob = new_green_knob;
        blue_knob = new_blue_knob;
        clock_nanosleep(CLOCK_MONOTONIC, 0, clock, NULL);
    }
}
void draw_screen(unsigned char* parlcd_mem_base) {
    int height = 50;
    int pixel_scale = 6;
    int start_y = 14;
    int start_x = 162;

    unsigned short** frame_buffer = calloc(480, sizeof(short*));
    for (int i = 0; i < 480; i++) {
        frame_buffer[i] = calloc(320, sizeof(short));
    }

    //S
    draw_vert_line(start_x, start_y, 27, pixel_scale, frame_buffer, 0);
    draw_vert_line(start_x, 56, 8, pixel_scale, frame_buffer, 0);
    start_x += pixel_scale;
    draw_horziontal_line(start_x, start_y, 7, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + 21, 7, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + 44, 7, pixel_scale, frame_buffer, 0);
    start_x += 7;
    draw_vert_line(start_x, start_y, 8, pixel_scale, frame_buffer, 0);
    draw_vert_line(start_x, start_y + 21, 29, pixel_scale, frame_buffer, 0);
    start_x += pixel_scale*2;

    //P
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 0);
    start_x += 6;
    draw_horziontal_line(start_x, start_y, 16, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, 29, 16, pixel_scale, frame_buffer, 0);
    draw_vert_line(start_x+16-pixel_scale, start_y+pixel_scale, 9, pixel_scale, frame_buffer, 0);
    start_x += (16+6);

    //E
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 0);
    start_x += pixel_scale;
    draw_horziontal_line(start_x, start_y, 16, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + 16, 16, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + height - pixel_scale, 16, pixel_scale, frame_buffer, 0);
    start_x += 16 + pixel_scale;

    //E
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 0);
    start_x += pixel_scale;
    draw_horziontal_line(start_x, start_y, 16, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + 16, 16, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + height - pixel_scale, 16, pixel_scale, frame_buffer, 0);
    start_x += 16 + pixel_scale;

    //D
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y, 16, pixel_scale, frame_buffer, 0);
    draw_horziontal_line(start_x, start_y + height - pixel_scale, 16, pixel_scale, frame_buffer, 0);
    start_x += pixel_scale + 10;
    draw_vert_line(start_x, start_y + pixel_scale, height - 2*pixel_scale, pixel_scale, frame_buffer, 0);
    start_x += pixel_scale*2;

    //?
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

    //red 0.1x bottom left
    start_y = 211;
    start_x = 70;
    pixel_scale = 8;
    //0
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 1);
    draw_horziontal_line(start_x + pixel_scale, start_y, 10, pixel_scale, frame_buffer, 1);
    draw_horziontal_line(start_x + pixel_scale, start_y + height - pixel_scale, 10, pixel_scale, frame_buffer, 1);
    start_x += pixel_scale + 10;
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 1);
    //.
    start_x += pixel_scale*2;
    draw_square(start_x, start_y + height - pixel_scale, pixel_scale, frame_buffer, 1);
    //1
    start_x += pixel_scale*2;
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 1);
    for (int i = 0; i < 18; i++) {
        start_x--;
        start_y++;
        draw_square(start_x, start_y, pixel_scale, frame_buffer, 1);
    }
    start_x += 18;
    start_y -= 18;
    //x
    start_x += pixel_scale*2;
    draw_square(start_x, start_y + height - pixel_scale, pixel_scale, frame_buffer, 1);
    draw_square(start_x, start_y + height - 3*pixel_scale, pixel_scale, frame_buffer, 1);
    start_x += pixel_scale;
    draw_square(start_x, start_y + height - 2*pixel_scale, pixel_scale, frame_buffer, 1);
    start_x += pixel_scale;
    draw_square(start_x, start_y + height - pixel_scale, pixel_scale, frame_buffer, 1);
    draw_square(start_x, start_y + height - 3*pixel_scale, pixel_scale, frame_buffer, 1);
    //green 1x bottom center
    start_x = 230;
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 3);
    for (int i = 0; i < 18; i++) {
        start_x--;
        start_y++;
        draw_square(start_x, start_y, pixel_scale, frame_buffer, 3);
    }
    start_x += 18;
    start_y -= 18;
    //x
    start_x += pixel_scale*2;
    draw_square(start_x, start_y + height - pixel_scale, pixel_scale, frame_buffer, 3);
    draw_square(start_x, start_y + height - 3*pixel_scale, pixel_scale, frame_buffer, 3);
    start_x += pixel_scale;
    draw_square(start_x, start_y + height - 2*pixel_scale, pixel_scale, frame_buffer, 3);
    start_x += pixel_scale;
    draw_square(start_x, start_y + height - pixel_scale, pixel_scale, frame_buffer, 3);
    draw_square(start_x, start_y + height - 3*pixel_scale, pixel_scale, frame_buffer, 3);
    //blue 10x bottom right
    //1
    start_x = 350;
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 2);
    for (int i = 0; i < 18; i++) {
        start_x--;
        start_y++;
        draw_square(start_x, start_y, pixel_scale, frame_buffer, 2);
    }
    start_x += 18;
    start_y -= 18;
    //0
    start_x += 2*pixel_scale;
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 2);
    draw_horziontal_line(start_x + pixel_scale, start_y, 10, pixel_scale, frame_buffer, 2);
    draw_horziontal_line(start_x + pixel_scale, start_y + height - pixel_scale, 10, pixel_scale, frame_buffer, 2);
    start_x += pixel_scale + 10;
    draw_vert_line(start_x, start_y, height, pixel_scale, frame_buffer, 2);
    //x
    start_x += pixel_scale*2;
    draw_square(start_x, start_y + height - pixel_scale, pixel_scale, frame_buffer, 2);
    draw_square(start_x, start_y + height - 3*pixel_scale, pixel_scale, frame_buffer, 2);
    start_x += pixel_scale;
    draw_square(start_x, start_y + height - 2*pixel_scale, pixel_scale, frame_buffer, 2);
    start_x += pixel_scale;
    draw_square(start_x, start_y + height - pixel_scale, pixel_scale, frame_buffer, 2);
    draw_square(start_x, start_y + height - 3*pixel_scale, pixel_scale, frame_buffer, 2);

    *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_CMD_o) = 0x2c;
    for (int i = 0; i < 320; i++) {
        for (int j = 0; j < 480; j++) {
            *(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_DATA_o) = frame_buffer[j][i];
        }
    }
}