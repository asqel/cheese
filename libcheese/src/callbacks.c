#include "libcheese_chess.h"

void call_piece_callback(board_t *board, int callback_id,
		piece_t *myself, piece_t *other)
{
	if (!myself || !myself->type->piece_callbacks[callback_id])
		return ;
	++board->callbacks_depth[callback_id];
	myself->type->piece_callbacks[callback_id](board, myself, other);
	--board->callbacks_depth[callback_id];
}
