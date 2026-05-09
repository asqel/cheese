#include "libpiss.h"

int	move_knight(board_t *board, piece_t *target, int y, int x)
{
	int		valid_move = 0;
	int		y_values[] = {-2, -2, 2, 2, -1, 1, -1, 1};
	int		x_values[] = {-1, 1, -1, 1, -2, -2, 2, 2};
	int		target_x, target_y;

	for (size_t i = 0; i < (sizeof(y_values) / sizeof(y_values[0])); i++) {
		target_y = y + y_values[i];
		target_x = x + x_values[i];
		if (target_y < 0 || target_x < 0 ||
			target_y >= board->height || target_x >= board->width)
			continue ;
		default_evaluate_move(board, target, target_y, target_x, &valid_move);
	}
	return (valid_move);
}
