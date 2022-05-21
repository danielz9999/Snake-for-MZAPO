#include <math.h>

#include "knob_parsing.h"

int parse_knob(int new_value, int old_value, bool* has_player_turned, int old_direction) {
    unsigned int relative_change = abs(old_value - new_value);
    if (relative_change > 1) {
        //Checks for char overflow
        if (relative_change > 200) {
            if (old_value < 100) {
                old_direction++;
            } else {
                old_direction--;
            }
        
        } else if (old_value > new_value) {
            old_direction++;
        } else {
            old_direction--;
        }
        //Loops direction to give range <1,4>
        if (old_direction == 5) {
            old_direction = 1;
        } else if (old_direction == 0) {
            old_direction = 4;
        }
        *(has_player_turned) = true;
        return old_direction;
    } else {
        return old_direction;
    }

}