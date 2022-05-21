#include <stdbool.h>

#ifndef KNOB_PARSING_H
#define KNOB_PARSING_H


//Returns direction based on values of knobs
int parse_knob(int new_value, int old_value, bool* has_player_turned, int old_direction);

#endif  /*KNOB_PARSING_H*/