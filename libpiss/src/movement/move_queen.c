#include "libpiss.h"

int	move_queen(board_t *board, piece_t *target, int y, int x)
{
	return (move_rook(board, target, y, x) + move_bishop(board, target, y, x));
}
