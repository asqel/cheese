#include "libcheese_chess.h"

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

void	reset_possible_moves(board_t *board)
{
	for (int j = 0; j < board->height; j++) {
		memset(board->default_locations[j], 0, board->width);
		for (int i = 0; i < board->width; i++)
			memset(board->default_moves[j][i], 0, board->nb_piece);
	}
	board->possible_moves = board->default_moves;
	board->possible_locations = board->default_locations;
}

piece_t	*get_tile_piece(board_t *board, int y, int x)
{
	if (board->tiles[y][x].nb_piece)
		return (board->tiles[y][x].pieces[0]);
	return (NULL);
}

void	free_possible_moves(board_t *board, char ***moves) {
	for (int j = 0; j < board->height; j++) {
		for (int i = 0; i < board->width; i++)
			free(moves[j][i]);
		free(moves[j]);
	}
	free(moves);
}

int	get_nb_pieces_on_tile(tile_t *tile, int color)
{
	int	res = 0;
	int must_match = (color > 0);

	if (color == BOARD)
		return (0);
	for (int i = 0; i < tile->nb_piece; i++) {
		int piece_color = tile->pieces[i]->type->color;
		if (must_match && piece_color == color)
			res++;
		else if (!must_match && piece_color != -color && piece_color != BOARD)
			res++;
		else if (!must_match && piece_color == BOARD && !tile->pieces[i]->invincible)
			res++;
	}
	return (res);
}
