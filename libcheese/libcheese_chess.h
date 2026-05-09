#ifndef LIBCHEESE_CHESS_H
#define LIBCHEESE_CHESS_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#define BOARD	0
#define WHITE	1
#define BLACK	2

typedef struct player_s
{
	int		nb_piece;
	int		nb_kings;
	int		king_in_check;
	int		color;
	int		compute_check;
}	player_t;

typedef struct {
	int			origin_x;
	int			origin_y;
	int			origin_id;
	int			target_x;
	int			target_y;
	int			target_id;
}	selector_t;

enum tile_types {
	REAL_TILE,
	MODIFIED_TILE,
	COPY_TILE
};

struct piece_s;
typedef struct {
	int	piece_type_id;
	int attack_power;
	int nb_move;
	int max_hp;
	int color;
	char character[5];
	char name[8];
	char is_cannibal;
	char is_king;
	char id;
	int (*default_move_func)(void *board, struct piece_s *target, int x, int y);
} piece_type_t;

typedef struct piece_s {
	piece_type_t	*type;
	uint16_t		piece_id;
	uint16_t		x; //TODO add support
	uint16_t		y;
	uint16_t		tile_id;
	uint16_t		hp;
	uint16_t		kill_count;
	uint16_t		move_counter;
	uint16_t		distance_moved;
	int16_t			attack_power;
	char			**possible_locations;
	char			***possible_moves;
	char			is_targeted;
	char			is_dead;
	char			can_move;
	char			invincible;
}	piece_t;

typedef struct {
	int			x;
	int			y;
	int			color;
	int			nb_piece;
	int			tile_type;
	int			is_blocked;
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
	char			***possible_moves;
	char			***default_moves;
	char			**possible_locations;
	char			**default_locations;
	selector_t		selector;
	move_logs_t		*logs;
	void			*special_tile;
	//tile_t			*promo_tile;
	int				debug;
}	board_t;

int	max(int a, int b);
int	min(int a, int b);

void default_move_piece(board_t *board, int y, int x);
void default_evaluate_move(board_t *board, piece_t *target, int y, int x, int *valid_move);
piece_t	*get_tile_piece(board_t *board, int y, int x);
void reset_possible_moves(board_t *board);
int get_nb_pieces_on_tile(tile_t *tile, int color);
void free_possible_moves(board_t *board, char ***moves);
void update_logs(board_t *board, piece_t *piece, piece_t *target);

#endif
