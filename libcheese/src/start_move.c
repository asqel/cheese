#include "libcheese_chess.h"

void start_move(board_t *board)
{
	selector_t	*s = &board->selector;
	tile_t	*origin_tile = &board->tiles[s->origin_y][s->origin_x];
	piece_t *selected_piece = origin_tile->pieces[s->origin_id];

	selected_piece->type->turn_function(board);
	reset_possible_moves(board);
	//update_logs(board);//, new_piece, target_piece);
}
