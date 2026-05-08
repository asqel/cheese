#include "libpiss.h"

int	move_king(board_t *board, piece_t *target, int y, int x)
{
	tile_t	*tile;
	int		valid_move = 0;

	if (get_nb_pieces_on_tile(&board->tiles[y][x], -target->type->color))
		default_evaluate_move(board, target, y, x, &valid_move);
	for (int y_offset = -1; y_offset < 2; y_offset++) {
		int	target_y = y + y_offset;
		if (target_y < 0 || target_y >= board->height)
			continue ;
		for (int x_offset = -1; x_offset < 2; x_offset++) {
			int	target_x = x + x_offset;
			if (target_x < 0 || target_x >= board->width)
				continue ;
			if (target_x == x && target_y == y)
				continue ;
			tile = &board->tiles[target_y][target_x];
			if (board->debug || !tile->nb_piece || get_nb_pieces_on_tile(tile, -target->type->color))
				default_evaluate_move(board, target, target_y, target_x, &valid_move);
		}
	}
	return (valid_move);
}
