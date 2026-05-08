#include "libpiss.h"

int	move_bishop(board_t *board, piece_t *target, int y, int x)
{
	tile_t	*tile;
	int		valid_move = 0;
	int		y_moves[] = {-1, -1, 1, 1};
	int		x_moves[] = {1, -1, -1, 1};
	int		target_x, target_y;

	for (size_t i = 0; i < (sizeof(y_moves) / sizeof(y_moves[0])); i++) {
		target_y = y;
		target_x = x;
		while (1) {
			target_y += y_moves[i];
			target_x += x_moves[i];
			if (target_y < 0 || target_x < 0 ||
				target_y >= board->height || target_x >= board->width)
				break ;
			tile = &board->tiles[target_y][target_x];
			if (board->debug)
				default_evaluate_move(board, target, target_y, target_x, &valid_move);
			if (!board->debug && tile->nb_piece) {
				if (get_nb_pieces_on_tile(tile, -target->type->color))
					default_evaluate_move(board, target, target_y, target_x, &valid_move);
				break ;
			}
			default_evaluate_move(board, target, target_y, target_x, &valid_move);
		}
	}
	return (valid_move);
}

