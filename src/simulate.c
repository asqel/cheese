#include "cheese.h"

board_t	*clone_board(board_t *board)
{
	board_t	*dest = malloc(sizeof(board_t));
	
	*dest = *board;
	dest->occupied_map = malloc((dest->height + 1) * sizeof(char *));
	dest->possible_moves = malloc((dest->height + 1) * sizeof(char *));
	if (!dest->occupied_map || !dest->possible_moves)
		exit(1);
	dest->possible_moves[dest->height] = 0;
	for (int i = 0; i < dest->height; i++) {
		dest->occupied_map[i] = malloc((dest->width + 1) * sizeof(char));
		dest->possible_moves[i] = malloc((dest->width + 1) * sizeof(char));
		if (!dest->occupied_map[i] || !dest->possible_moves[i])
			exit(1);
	}
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
		memcpy(cpy->occupied_map[j], src->occupied_map[j], src->width);
		memcpy(cpy->possible_moves[j], src->possible_moves[j], src->width);
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
	for (int j = 0; j < board->height; j++) {
		for (int i = 0; i < board->width; i++) {
			tile = &board->tiles[j][i];
			if (tile->nb_piece &&
				tile->pieces[0].color != color)
				update_possible_moves(board, j, i);
		}
	}
	for (int j = 0; j < board->height; j++) {
		for (int i = 0; i < board->width; i++) {
			tile = &board->tiles[j][i];
			if (!board->possible_moves[j][i] || !tile->nb_piece)
				continue ;
			if (tile->pieces[0].type == KING &&
				tile->pieces[0].color == color)
				check = 1;
		}
	}
	return (check);
}
