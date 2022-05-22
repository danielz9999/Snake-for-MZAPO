#ifndef GENERATE_FRUIT_H
#define GENERATE_FRUIT_H

void generate_fruit(unsigned char **frame_buffer, int* last_fruit_x_coord, int* last_fruit_y_coord, int snake_size);

void fruit_get(unsigned int* score, unsigned int* blue_score, unsigned char* mem_base, struct timespec* clock, bool is_two_players, int player);

void timer_decrement(struct timespec* clock);

#endif  /*GENERATE_FRUIT_H*/