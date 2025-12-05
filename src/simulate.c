#include "cheese.h"

board_t	*clone_board(board_t *board)
{
	board_t	*dest = malloc(sizeof(board_t));
	
	*dest = *board;
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

void	sync_boards(board_t *cpy, board_t *src)
{
	for (int j = 0; j < src->height; j++) {
		for (int i = 0; i < src->width; i++)
			cpy->tiles[j][i] = src->tiles[j][i];
		cpy->tiles[j][0] = src->tiles[j][0];
	}
	cpy->selector.origin_x = src->selector.origin_x;
	cpy->selector.origin_y = src->selector.origin_y;
	cpy->selector.origin_id = src->selector.origin_id;
}

int	king_in_check(board_t *board_base, int color)
{
	board_t	*board;
	int		check = 0;
	tile_t	*tile;

   	board = board_base->copy_board;
	reset_possible_moves(board);
	for (int j = 0; j < board->height; j++) {
		for (int i = 0; i < board->width; i++) {
			tile = &board->tiles[j][i];
			for (int k = 0; k < tile->nb_piece; k++) {
				if (tile->pieces[k]->color != color) 
					simulate_piece(board, tile->pieces[k]);
			}
		}
	}
	for (int j = 0; j < board->height; j++) {
		for (int i = 0; i < board->width; i++) {
			tile = &board->tiles[j][i];
			if (!board->possible_moves[j][i] || !tile->nb_piece)
				continue ;
			if (tile->pieces[0]->type == KING &&
				tile->pieces[0]->color == color)
				check = 1;
		}
	}
	return (check);
}
