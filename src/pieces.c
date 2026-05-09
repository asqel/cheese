#include "cheese.h"

/* y < 0 && x < 0 = all pieces
   y < 0 && x >= 0 = only x color
   y > 0 && x <= 0 = all but y color
*/
int	update_possible_moves(board_t *board, int y, int x) {
	if (y >= 0 && x >= 0) {
		board->selector.origin_x = x;
		board->selector.origin_y = y;
		piece_t	*piece = board->tiles[y][x].pieces[board->selector.origin_id];
		board->selected_piece = piece;
		board->possible_moves = piece->possible_moves;
		board->possible_locations = piece->possible_locations;
		return (piece->can_move);
	}
	for (int p = 0; p < board->nb_piece; p++) {
		piece_t	*piece = board->pieces[p];
		if (piece->is_dead ||
			(x >= 0 && piece->type->color != x) ||
			(y >= 0 && piece->type->color == x))
			continue ;
		piece->can_move = simulate_piece(board, piece) != 0;
		x++;
		x--;
	}
	return (1);
}

int	simulate_piece(board_t *board, piece_t *target) {
	board->selector.origin_x = target->x;
	board->selector.origin_y = target->y;
	board->selector.origin_id = target->tile_id;
	for (int j = 0; j < board->height; j++) {
		memset(target->possible_locations[j], 0, board->width);
		for (int i = 0; i < board->width; i++)
			memset(target->possible_moves[j][i], 0, max(1, board->tiles[j][i].nb_piece));
	}
	if (!target->type->default_move_func)
		return (1);
	return ((target->type->default_move_func)(board, target, target->y, target->x));
}
