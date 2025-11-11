#ifndef CHEESE_H
#define CHEESE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdint.h>

#define CURSOR_UP "\033[A"
#define CURSOR_DOWN "\033[B"
#define CURSOR_RIGHT "\033[C"
#define CURSOR_LEFT "\033[D"
#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"
#define BLACK_BG	"\033[40m"
#define WHITE_BG	"\033[47m"
#define BLUE_BG		"\033[43m"

#define WHITE	0
#define BLACK	1

enum pieces {
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING
};

typedef struct {
	int			origin_x;
	int			origin_y;
	int			origin_id;
	int			target_x;
	int			target_y;
}	selector_t;

typedef struct {
	int			type;
	int			color;
	int			kill_count;
	int			move_counter;
	char		character[5];
}	piece_t;

typedef struct {
	int			color;
	int			nb_piece;
	piece_t		*pieces;
}	tile_t;

typedef struct board_s {
	int			width;
	int			height;
	int			white_pieces;
	int			black_pieces;
	int			white_checked;
	int			black_checked;
	tile_t		**tiles;
	char		**occupied_map;
	char		**possible_moves;
	selector_t	selector;
	board_s		copy_board;
}	board_t;

void	free_board(board_t *board, int free_char);
void	init_board(char *filepath, board_t *board);
int		update_possible_moves(board_t *board, int y, int x);

//simulations
board_t	clone_board(board_t *board);
int		king_in_check(board_t *board, int color);

//utils
char	*get_tile_pieces(board_t *board, int x, int y);
char	*strjoin(char *s1, char *s2, int free_s1);
void	get_cursor_position(int *x, int *y);
void	reset_possible_moves(board_t *board);
int		max(int a, int b);
int		min(int a, int b);

//buffers
typedef struct {
	void *data;
	size_t len;
	size_t alloc_len;
} buffer_t;

int		buffer_append(buffer_t *buffer, void *data, size_t len);
void	buffer_remove(buffer_t *buffer, size_t start, size_t len);
void 	buffer_free(buffer_t *buffer);
void	disable_raw_mode(void);
void	enable_raw_mode(void);
char	read_char(void);

#endif
