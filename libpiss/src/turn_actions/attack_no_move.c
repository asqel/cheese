#include "libpiss.h"

void attack_no_move(board_t *board)
{
	selector_t	*s = &board->selector;
	tile_t	*origin_tile = &board->tiles[s->origin_y][s->origin_x];
	tile_t	*target_tile = &board->tiles[s->target_y][s->target_x];

	if (target_tile->nb_piece <= s->target_id)
		return ;

	piece_t *selected_piece = origin_tile->pieces[s->origin_id];
	piece_t	*target_piece = target_tile->pieces[s->target_id];

	damage_piece(board, target_tile, target_piece,
			selected_piece, selected_piece->attack_power);
}
