#ifndef CHEESE_H
#define CHEESE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdint.h>

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

#define WHITE	1
#define BLACK	2

enum pieces {
	KING,
	QUEEN,
	ROOK,
	BISHOP,
	KNIGHT,
	PAWN
};

typedef struct {
	int			origin_x;
	int			origin_y;
	int			origin_id;
	int			target_x;
	int			target_y;
	int			target_id;
}	selector_t;

typedef struct {
	uint64_t	piece_id;
	int			x; //TODO add support
	int			y;
	int			tile_id;
	int			is_targeted;
	int			is_dead;
	int			type;
	int			color;
	int			kill_count;
	int			move_counter;
	int			distance_moved;
	char		**possible_locations;
	char		***possible_moves;
	int			can_move;
	char		character[5];
}	piece_t;

enum tile_types {
	REAL_TILE,
	MODIFIED_TILE,
	COPY_TILE
};

typedef struct {
	int			x;
	int			y;
	int			color;
	int			nb_piece;
	int			tile_type;
	int			is_targeted;
	piece_t		**pieces;
}	tile_t;

typedef struct {
	piece_t		*piece;
	piece_t		*target_piece;
	int			extra_info;
	int			origin_x;
	int			origin_y;
	int			target_x;
	int			target_y;
	int			color;
}	move_infos_t;

#define MAX_LOG	512
#define MAX_PLAYER 8
typedef struct {
	int				last_color_played;
	uint64_t		nb_move;
	uint64_t		color_moves[MAX_PLAYER];
	move_infos_t	color_logs[MAX_PLAYER][MAX_LOG];
	move_infos_t	*global_log[MAX_LOG];
	move_infos_t	*last_move;
}	move_logs_t;

typedef struct player_s
{
	int		nb_piece;
	int		nb_kings;
	int		king_in_check;
	int		color;
}	player_t;

#define MAX_SIMU_MOVES	16
typedef struct board_s
{
	int				width;
	int				height;
	int				nb_player;
	player_t		*players;
	piece_t			**pieces;
	piece_t			*selected_piece;
	int				nb_piece;
	tile_t			**tiles;
	char			**occupied_map;
	char			***default_moves;
	char			**possible_locations;
	char			***possible_moves;
	selector_t		selector;
	move_logs_t		*logs;
	move_infos_t	*simu_changes;
	int				simu_change_index;
	void			*special_tile;
	//tile_t			*promo_tile;
	int				debug;
	struct board_s	*copy_board;
}	board_t;

piece_t	*get_piece(int index);
piece_t	*set_piece(int c);

int		play(board_t *board);
void	free_board(board_t *board);
void	free_possible_moves(board_t *board, char ***moves);
void	init_board(char *filepath, board_t *board);
int		update_possible_moves(board_t *board, int y, int x);
int		simulate_piece(board_t *board, piece_t *target);
int		promo_menu(int y, int color, board_t *board);
int		choose_tile_piece_menu(board_t *board, tile_t *tile, int color);
int		choose_target_piece(board_t *board, piece_t *src, tile_t *target_tile);
void	update_logs(board_t *board, piece_t *piece, piece_t *target);
void	move_piece(board_t *board, int y, int x);

//simulations
board_t	*clone_board(board_t *board);
void	sync_boards(board_t *cpy, board_t *src);
int		king_in_check_simu(board_t *board_base, int color);
int		king_in_check(board_t *board, int color);

//utils
void	highlight_board(board_t *board, int y, int x);
piece_t	*get_tile_piece(board_t *board, int x, int y);
int		get_nb_pieces_on_tile(tile_t *tile, int color);
char	*strjoin(char *s1, char *s2, int free_s1);
void	get_cursor_position(int *x, int *y);
void	reset_possible_moves(board_t *board);
int		max(int a, int b);
int		min(int a, int b);
char	*sha256(char *str);

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
piece_t	*create_piece(char piece, int index);

void	print_error(char *error, int ret);

#endif
