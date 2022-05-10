#define _POSIX_C_SOURCE 200112L

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// guarantee that fruit will not spawn inside of snake is not implemented - depending on snake representation
void generate_fruit(unsigned char *frame_buffer[480][320], int last_fruit_x_coord, int last_fruit_y_coord) {
    // variables
    int width = 480;
    int height = 320;
    // generate random coords
    int random_x_coord = rand() % (width - 1);
    int random_y_coord = rand() % (height - 1);
    //calculate distance from previous fruit
    double x_distance = abs(last_fruit_x_coord - random_x_coord);
    double y_distance = abs(last_fruit_y_coord - random_y_coord);
    int fruit_distance = sqrt(pow(x_distance, 2) + pow(y_distance, 2));
    //place fruit into buffer
    if(fruit_distance >= 7) {
        if(frame_buffer[random_x_coord][random_y_coord] == 0) {
            frame_buffer[random_x_coord][random_y_coord] = 10;
        } else {
            generate_fruit(frame_buffer, last_fruit_x_coord, last_fruit_y_coord);
        }
    } else {
        generate_fruit(frame_buffer, last_fruit_x_coord, last_fruit_y_coord);
    }
}
