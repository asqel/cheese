#include "libpiss.h"

void default_move_piece(board_t *board)
{
	selector_t	*s = &board->selector;
	tile_t	*origin_tile = &board->tiles[s->origin_y][s->origin_x];
	tile_t	*target_tile = &board->tiles[s->target_y][s->target_x];
	piece_t *selected_piece = origin_tile->pieces[s->origin_id];
	piece_t	*target_piece = NULL;

	piece_t *new_piece = simple_move(board, selected_piece, s->target_y, s->target_x);
	if ((target_tile->nb_piece > 1) && (selected_piece->type->is_cannibal || 
		selected_piece->type->color != target_tile->pieces[s->target_id]->type->color)) {
		target_piece = target_tile->pieces[s->target_id];
		damage_piece(board, target_tile, target_piece, new_piece, new_piece->attack_power);
	}
}
