#include "stm32f3xx.h"       

void loop();
void _n_fps_loop(uint32_t, uint8_t);
void setup_game();
void clear_board();
void write_out();
void spawn_entities();
void draw_bar();
void draw_ball(char);
void udpdate_ball_physics();
void update_y_movement();
void update_x_movement();
void clear_enemy(uint16_t);