#include "cheese.h"

board_t	*clone_board(board_t *board) {
	board_t	*dest = malloc(sizeof(board_t));
	
	*dest = *board;
	board->simu_changes = NULL;
	dest->simu_changes = malloc((MAX_SIMU_MOVES + 1) * sizeof(move_infos_t));
	if (!dest->simu_changes)
		exit(1);
	dest->occupied_map = NULL;
	dest->tiles = malloc((dest->height + 1) * sizeof(tile_t *));
	if (!dest->tiles)
		exit(1);
	for (int j = 0; j < dest->height; j++) {
		dest->tiles[j] = malloc(dest->width * sizeof(tile_t));
		if (!dest->tiles[j])
			exit(1);
	}
	dest->tiles[dest->height] = NULL;
	return (dest);
}

void	sync_boards(board_t *cpy, board_t *src) {
	for (int j = 0; j < src->height; j++)
		for (int i = 0; i < src->width; i++)
			cpy->tiles[j][i] = src->tiles[j][i];
	cpy->selector.origin_x = src->selector.origin_x;
	cpy->selector.origin_y = src->selector.origin_y;
	cpy->selector.origin_id = src->selector.origin_id;
}

int	king_in_check_simu(board_t *board_base, int color) {
	board_t	*board;
	tile_t	*tile;

   	board = board_base->copy_board;
	reset_possible_moves(board);
	for (int j = 0; j < board->height; j++) {
		for (int i = 0; i < board->width; i++) {
			for (int k = 0; k < board->tiles[j][i].nb_piece; k++) {
				piece_t	*piece = board->tiles[j][i].pieces[k];
				if (piece->color != color)
					simulate_piece(board, piece);
			}
		}
	}
	int	check = 0;
	for (int j = 0; j < board->height; j++) {
		for (int i = 0; i < board->width; i++) {
			tile = &board->tiles[j][i];
			if (!board->possible_moves[j][i] || !tile->nb_piece)
				continue ;
			for (int k = 0; k < tile->nb_piece; k++)
				if (tile->pieces[k]->type == KING &&
					tile->pieces[k]->color == color)
					check++;
		}
	}
	return (check == board_base->players[color].nb_kings);
}

int	king_in_check(board_t *board, int color) {
	reset_possible_moves(board);
	for (int p = 0; p < board->nb_piece; p++) {
		piece_t	*piece = board->pieces[p];
		if (piece->is_dead || piece->color == color)
			continue ;
		for (int j = 0; j < board->height; j++) {
			for (int i = 0; i < board->width; i++) {
				if (piece->possible_moves[j][i])
					board->possible_moves[j][i] = 1;
			}
		}
	}
	int	check = 0;
	for (int p = 0; p < board->nb_piece; p++) {
		piece_t	*piece = board->pieces[p];
		if (piece->is_dead || piece->color != color)
			continue ;
		piece->is_targeted = 0;
		if (!board->possible_moves[piece->y][piece->x])
			continue ;
		piece->is_targeted = 1;
		if (piece->type == KING)
			check = 1;
	}
	return (check == board->players[color].nb_kings);
}
