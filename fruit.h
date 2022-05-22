#ifndef FRUIT_H
#define FRUIT_H
//Generates a new fruit, with a minimal distance from the last one, fruit size == snake size
void generate_fruit(unsigned char **frame_buffer, int* last_fruit_x_coord, int* last_fruit_y_coord, int snake_size);
//The logic behind picking up fruit, mainly lighting up LED lights and the RGB diode, but also eg deleting the picked up fruit
void fruit_get(unsigned int* score, unsigned int* blue_score, unsigned char* mem_base, struct timespec* clock, bool is_two_players, int player);
//Called by fruit_get, makes the game go faster
void timer_decrement(struct timespec* clock);

#endif  /*FRUIT_H*/