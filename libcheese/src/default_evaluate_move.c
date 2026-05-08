#include "libcheese_chess.h"

void	default_evaluate_move(board_t *board, piece_t *target, int y, int x, int *valid_move)
{
	tile_t		*target_tile = &board->tiles[y][x];

	for (int p = 0; p < max(1, target_tile->nb_piece); p++) {
		int	compute_move = board->debug;
		if (target_tile->nb_piece < 2)
			compute_move = 1;
		else if (target_tile->pieces[p]->type->color != target->type->color)
			compute_move = 1;
		if (!compute_move)
			continue ;
		*valid_move = 1;
		target->possible_moves[y][x][p] += target->attack_power;
		target->possible_locations[y][x] = 1;
	}
}
