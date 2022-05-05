#define _POSIX_C_SOURCE 200112L

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"

// guarantee that fruit will not spawn inside of snake is not implemented - depending on snake representation
int generate_fruit() {
    // variables
    int x, y;
    int width = 480;
    int height = 320;

    void *parlcd_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    // check if everything is correct
    if(parlcd_base == NULL) {
        fprintf(stderr, "No display.");
        exit(1);
    }

    *(volatile uint16_t*)(parlcd_base + PARLCD_REG_CMD_o) = 0x2c;
    // generate random coords
    int random_x_coord = rand() % width;
    int random_y_coord = rand() % height;
    // for cycle to get to 3 x 3 coords that represent "fruit"
    for(int x = 0; x <= random_x_coord + 1; x++) {
        for(int y = 0; y <= random_y_coord + 1; y++) {
            // if we reached the random coords add color
            if(y == random_y_coord - 1 || y == random_y_coord || y == random_y_coord + 1) {
                if(x == random_x_coord - 1 || x == random_x_coord || x == random_x_coord + 1) {
                    // color the coords
                    *(volatile uint16_t*)(parlcd_base + PARLCD_REG_DATA_o) = 0x001F;
                }
            }
        }
    }
    return 0;
}
