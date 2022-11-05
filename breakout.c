#include <stdio.h>
#include "breakout.h"
#include "USART.h"

#define MAX_WIDTH  	84
#define MAX_HEIGHT 	48

#define NO_ENEMIES	24 // Must be mutiples of 8 since 8 enemies will spawn per line 24 enemies means = 3 lines
#define FPS					30
#define ENEMY_H			5
#define ENEMY_C			'#'

#define BAR_W				16
#define BAR_H				3
#define BAR_C				'@'
#define VEL					1
#define BALL_C			'O'

char board[MAX_WIDTH * MAX_HEIGHT];
volatile uint8_t bar_pos = 0;
uint16_t ball_pos = 2058;
uint16_t ticks = 0;
uint8_t	dir_x = 0;
uint8_t dir_y =	1;
uint8_t score = 0;

uint16_t bar_init_offset = (MAX_WIDTH * MAX_HEIGHT) - (MAX_WIDTH * 5);

void setup_game() {
  clear_board();
	spawn_entities();
	
	
	ball_pos = 2058;
}

void loop() {
	draw_bar();
	
	draw_ball(' ');
	udpdate_ball_physics();
	draw_ball(BALL_C);
	
  write_out();
}

// Helper functions
void spawn_entities() {
	uint16_t init_offset = MAX_WIDTH * 3;
	uint8_t rows = NO_ENEMIES / 8;
	
	for (uint8_t i = 0; i < rows; i++) {
		uint16_t offset = init_offset + (i * MAX_WIDTH) + 3;  // 1 -> init_offset + (1 * 84) + 3;
		uint16_t row_offset = (i * (ENEMY_H * MAX_WIDTH));  	// 1 -> (1 * 5 * 84)
		
		for (uint8_t j = 0; j < 8; j++) {
			uint16_t col_offset = 0;
			
			if (j != 0) {
				col_offset = (j * 10);
			}
			
			uint16_t coffset = offset + col_offset + row_offset;
			
			for (uint16_t k = 0; k < ENEMY_H; k++) {
				uint16_t k_offset = (k * MAX_WIDTH);
				board[coffset + 0 + k_offset] = ENEMY_C;
				board[coffset + 1 + k_offset] = ENEMY_C;
				board[coffset + 2 + k_offset] = ENEMY_C;
				board[coffset + 3 + k_offset] = ENEMY_C;
				board[coffset + 4 + k_offset] = ENEMY_C;
				board[coffset + 5 + k_offset] = ENEMY_C;
				board[coffset + 6 + k_offset] = ENEMY_C;
				board[coffset + 7 + k_offset] = ENEMY_C;
			}
		}
	}
	
	bar_pos = (MAX_WIDTH / 2) - (BAR_W / 2);
	draw_bar();
}

void draw_bar() {
	for (uint8_t i = 0; i < MAX_WIDTH * 3; i++) {
		board[bar_init_offset + i] = ' ';
	}
	
	for (uint8_t i = 0; i < BAR_W; i++) {
		board[bar_init_offset + bar_pos + i] = BAR_C;
		board[bar_init_offset + MAX_WIDTH + bar_pos + i] = BAR_C;
		board[bar_init_offset + (MAX_WIDTH * 2) + bar_pos + i] = BAR_C;
	}
}

void draw_ball(char c) {
	for(uint8_t i = 0; i < 3; i++) {
		board[ball_pos + MAX_WIDTH - 1] = c;
		board[ball_pos + MAX_WIDTH] = c;
		board[ball_pos + MAX_WIDTH + 1] = c;
	
		board[ball_pos - 1] = c;
		board[ball_pos] = c;
		board[ball_pos + 1] = c;
	
		board[ball_pos - MAX_WIDTH - 1] = c;
		board[ball_pos - MAX_WIDTH] = c;
		board[ball_pos - MAX_WIDTH + 1] = c;
	}
}

void udpdate_ball_physics() {
	update_y_movement();
	update_x_movement();
}

void update_y_movement() {
	if (dir_y && (ball_pos - (2 * MAX_WIDTH)) < 0) {
		dir_y = 0;
		return;
	}
	
	if (!dir_y && (ball_pos + (2 * MAX_WIDTH)) > (MAX_HEIGHT * MAX_WIDTH)) {
		dir_y = 1;
		return;
	}
	
	ball_pos += dir_y ? -MAX_WIDTH : MAX_WIDTH;
	
	if (board[ball_pos] == ENEMY_C) {
		dir_y = !dir_y;
		clear_enemy(ball_pos);
	}
}

void update_x_movement() {
	if (dir_x && (ball_pos + 3) % MAX_WIDTH == 0) {
		dir_x = 0;
		return;
	}
	
	if (!dir_x && (ball_pos - 2) % MAX_WIDTH == 0) {
		dir_x = 1;
		return;
	}
	
	ball_pos += dir_x ? 1 : -1;
}


void _n_fps_loop(uint32_t freq, uint8_t bpos) {
	bar_pos = bpos;
	
	// Executes loop every 30 fps (SystemClock / FPS) = Ticks required to draw 1 frame
	ticks++;
	uint16_t split_ticks = freq / FPS; 
	if (ticks > split_ticks) {
		loop();
		ticks = 0;
	}
}
	
void clear_board() {
  for (int i = 0; i < MAX_HEIGHT * MAX_WIDTH; i++) {
    board[i] = ' ';
  }
}

void clear_enemy(uint16_t pos) {
	
}

void write_out() {
	USART_putString("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n");

  for (int i = 0; i < MAX_HEIGHT; i++) {
    char str[MAX_WIDTH + 1];
    for (int j = 0; j < MAX_WIDTH; j++) {
      int idx = (i * MAX_WIDTH) + j;
      str[j] = board[idx];
    }	
		USART_putString(str);
		USART_putString("\r\n");
  }
}