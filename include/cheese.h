#ifndef CHEESE_H
#define CHEESE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <string.h>
#include <stdint.h>
#include "../libcheese/libcheese_chess.h"

#define LIBPISS "./libs/libpiss.so"
#define PROMO_OFFSET	5
#define MAX_TILE_PIECE	(16 + 1)

#define CURSOR_UP "\033[A"
#define CURSOR_DOWN "\033[B"
#define CURSOR_RIGHT "\033[C"
#define CURSOR_LEFT "\033[D"
#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"
#define BLACK_BG	"\033[40m"
#define WHITE_BG	"\033[47m"
#define BLUE_BG		"\033[43m"
#define RED_BG		"\033[41m"

typedef struct {
	piece_type_t default_conf;
	piece_type_t **piece_types;
} config_file_t;

enum pieces {
	KING,
	QUEEN,
	ROOK,
	BISHOP,
	KNIGHT,
	PAWN,
	BLOCK
};

#define MAX_SIMU_MOVES	16

piece_t	*get_piece(int index);
piece_t	*set_piece(int c);

int		play(board_t *board);
void	free_board(board_t *board);
void	init_board(char *filepath, board_t *board);
int		update_possible_moves(board_t *board, int y, int x);
int		simulate_piece(board_t *board, piece_t *target);
piece_t	*create_piece(char piece, int index);
void	print_error(char *error, int ret);
int		promo_menu(int y, int color, board_t *board);
int		choose_tile_piece_menu(board_t *board, tile_t *tile, int color);
int		choose_target_piece(board_t *board, piece_t *src, tile_t *target_tile);
void	update_logs(board_t *board, piece_t *piece, piece_t *target);

//config files
void parse_config_file(char *filepath);
void init_default_config(piece_type_t *piece);
piece_type_t *update_current_piece(char *line, size_t line_size, int is_default);

//simulations
void	sync_boards(board_t *cpy, board_t *src);
int		king_in_check(board_t *board, int color);

//utils
void	highlight_board(board_t *board, int y, int x);
char	*strjoin(char *src, char *to_add);
void	get_cursor_position(int *x, int *y);
void	reset_possible_moves(board_t *board);
char	*sha256(char *str);
size_t	strlen_utf8(char *str);

//buffers
typedef struct {
	void *data;
	size_t len;
	size_t alloc_len;
} buffer_t;

char	read_char(void);
int		buffer_append(buffer_t *buffer, void *data, size_t len);
void	buffer_remove(buffer_t *buffer, size_t start, size_t len);
void 	buffer_free(buffer_t *buffer);
void	disable_raw_mode(void);
void	enable_raw_mode(void);
char *read_passwd();
int terminal_set_canon(int enable);
int terminal_set_echo(int enable);
void restore_tty();
void save_tty();
void terminal_set_screen(int is_alt);
void terminal_get_pos(int *x, int *y);
int terminal_set_ctrl_key(char key, int state);
void terminal_get_size(int *w, int *h);
void terminal_clear(int fflush);
char *terminal_get_input();
int terminal_set_block(int state);
void terminal_set_cursor(int state);
void terminal_draw_str(char *str, int x, int y, int flush);
void terminal_draw_str_centered(char *str, int y, int flush);
void terminal_draw_strarr(char **str, int x, int y, int flush);
void terminal_set_flush(int state);
void terminal_draw_strarr_centered(char **str, int y, int flush);

extern config_file_t g_config_file;

#endif
