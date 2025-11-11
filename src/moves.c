#include "cheese.h"

void	remove_piece(tile_t *target, int id, int free_pieces)
{
	for (int i = (id + 1); i < target->nb_piece; i++)
		target->pieces[i - 1] = target->pieces[i];
	if (!--target->nb_piece && free_pieces) {
		free(target->pieces);
		target->pieces = NULL;
	}
}

void	move_piece(board_t *board, int y, int x)
{
	board->selector.target_y = y;
	board->selector.target_x = x;
	tile_t	*origin_tile = &board->tiles[board->selector.origin_y][board->selector.origin_x];
	tile_t	*target_tile = &board->tiles[board->selector.target_y][board->selector.target_x];
	piece_t selected_piece = origin_tile->pieces[0];//board->selector.origin_id];

	for (int i = (board->selector.origin_id + 1); i < origin_tile->nb_piece; i++)
		origin_tile->pieces[i - 1] = origin_tile->pieces[i];
	if (!--origin_tile->nb_piece && board->copy_board) {
		free(origin_tile->pieces);
		origin_tile->pieces = NULL;
	}
	if (target_tile->nb_piece) {
		remove_piece(target_tile, 0, board->copy_board != NULL);
		selected_piece.kill_count++;
	}
	selected_piece.move_counter++;
	if (!board->copy_board) {
		piece_t	*real_pieces = target_tile->pieces;
		target_tile->pieces = malloc(sizeof(piece_t) *
				(target_tile->nb_piece + 1));
		memcpy(target_tile->pieces, real_pieces, target_tile->nb_piece * sizeof(piece_t));
	}
	else
		target_tile->pieces = realloc(target_tile->pieces,
				sizeof(piece_t) * (target_tile->nb_piece + 1));
	if (!target_tile->pieces)
		exit(1);
	target_tile->pieces[target_tile->nb_piece++] = selected_piece;
	reset_possible_moves(board);
}

void	highlight_board(board_t *board, int y, int x)
{
	int cursor_y;
	int	cursor_x;

	get_cursor_position(&cursor_x, &cursor_y);
	write(1, "\e[?25l", 6);
	for (int j = y; j > 0; j--)
		for (int k = 0; k < 2; k++)
			printf("%s", CURSOR_UP);
	for (int i = x; i > 0; i--)
		for (int k = 0; k < 4; k++)
			printf("%s", CURSOR_LEFT);
	for (int j = 0; j < board->height; j++) {
		printf("\r%s%s", CURSOR_RIGHT, CURSOR_RIGHT);
		for (int i = 0; i < board->width; i++) {
			printf("%s%s%s", board->possible_moves[j][i] ? BLUE_BG : BLACK_BG,
					get_tile_pieces(board, i, j), BLACK_BG);
			for (int k = 0; k < 3; k++)
				printf("%s", CURSOR_RIGHT);
		}
		for (int i = 0; i < 2; i++)
			printf("%s", CURSOR_DOWN);
	}
	printf("\033[%d;%dH\e[?25h", cursor_y, cursor_x);
	fflush(stdout);
}
