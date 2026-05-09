#include "cheese.h"

void	sync_boards(board_t *cpy, board_t *src) {
	for (int j = 0; j < src->height; j++)
		for (int i = 0; i < src->width; i++)
			cpy->tiles[j][i] = src->tiles[j][i];
	cpy->selector.origin_x = src->selector.origin_x;
	cpy->selector.origin_y = src->selector.origin_y;
	cpy->selector.origin_id = src->selector.origin_id;
}

int	king_in_check(board_t *board, int color) {
	reset_possible_moves(board);
	for (int p = 0; p < board->nb_piece; p++) {
		piece_t	*piece = board->pieces[p];
		if (piece->is_dead || piece->type->color == color)
			continue ;
		for (int j = 0; j < board->height; j++) {
			for (int i = 0; i < board->width; i++) {
				tile_t	*target_tile = &board->tiles[j][i];
				for (int t = 0; t < max(1, target_tile->nb_piece); t++) {
					if (!piece->possible_moves[j][i][t])
						continue ;
					board->possible_moves[j][i][t] = 1;
					target_tile->is_targeted = 1;
				}
			}
		}
	}
	int	check = 0;
	for (int p = 0; p < board->nb_piece; p++) {
		piece_t	*piece = board->pieces[p];
		if (piece->is_dead || piece->type->color != color)
			continue ;
		piece->is_targeted = 0;
		if (!board->possible_moves[piece->y][piece->x][piece->tile_id])
			continue ;
		piece->is_targeted = 1;
		if (piece->type == KING)
			check = 1;
	}
	return (check == board->players[color].nb_kings);
}
