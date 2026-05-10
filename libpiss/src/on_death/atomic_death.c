#include "libpiss.h"

static void damage_tile(board_t *board, tile_t *tile, piece_t *myself)
{
	int i = 0;
	while (i < tile->nb_piece) {
		int killed = damage_piece(board, tile, tile->pieces[i], myself, 999);
		if (!killed)
			i++;
	}
}

void atomic_death(board_t *board, piece_t *myself, piece_t *attacker)
{
	(void)attacker;

	if (board->callbacks_depth[ON_DEATH] > 1)
		return ;
	for (int y = -1; y < 2; y++) {
		int tile_y = myself->y + y;

		if (tile_y < 0 || tile_y >= board->height)
			continue ;

		for (int x = -1; x < 2; x++) {
			int tile_x = myself->x + x;

			if (tile_x < 0 || tile_x >= board->width)
				continue ;

			damage_tile(board, &board->tiles[tile_y][tile_x], myself);
		}
	}
}
