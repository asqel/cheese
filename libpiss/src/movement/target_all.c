#include "libpiss.h"

int target_all(board_t *board, piece_t *target, int y, int x)
{
	(void)target;

	int valid_move = 0;
	for (int j = 0; j < board->height; j++) {
		for (int i = 0; i < board->width; i++) {
			if (j == y && i == x)
				continue ;
			default_evaluate_move(board, target, j, i, &valid_move);
		}
	}
	return (valid_move);
}
