#include "cheese.h"

void	remove_piece(tile_t *target, int id, board_t *board)
{
	if (target->pieces[id]->type == KING && board->copy_board) {
		if (target->pieces[id]->color == WHITE)
			board->white_kings--;
		else
			board->black_kings--;
	}
	if (board->copy_board) {
		for (int i = (id + 1); i < target->nb_piece; i++) {
			target->pieces[i - 1] = target->pieces[i];
		}
	}
	if (!--target->nb_piece && board->copy_board) {
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
	piece_t *selected_piece = origin_tile->pieces[board->selector.origin_id];
	piece_t	*target_piece = NULL;
	board->selector.target_id = 0;

	if (board->copy_board && (target_tile->nb_piece > 1) &&
		get_nb_pieces_on_tile(target_tile, !selected_piece->color))
		board->selector.target_id = choose_tile_piece_menu(board, target_tile,
				!selected_piece->color);

	if (!--origin_tile->nb_piece && board->copy_board) {
		free(origin_tile->pieces);
		origin_tile->pieces = NULL;
	}
	if (target_tile->nb_piece &&
		selected_piece->color != target_tile->pieces[board->selector.target_id]->color) {
		if (board->copy_board)
			target_piece = target_tile->pieces[board->selector.target_id];
		remove_piece(target_tile, board->selector.target_id, board);
	}
	else if (selected_piece->type == PAWN && !target_tile->nb_piece &&
		(board->selector.target_x != board->selector.origin_x)) {
		int y_pos = selected_piece->color == WHITE ? -1 : 1;
		if (board->copy_board)
			target_piece = board->tiles[y + y_pos][x].pieces[board->selector.target_id];
		remove_piece(&board->tiles[y + y_pos][x], board->selector.target_id, board);
	}
	if (!board->copy_board) {
		piece_t	**real_pieces = target_tile->pieces;
		target_tile->pieces = malloc(sizeof(piece_t *) *
				(target_tile->nb_piece + 1));
		memcpy(target_tile->pieces, real_pieces, target_tile->nb_piece * sizeof(piece_t *));
	}
	else
		target_tile->pieces = realloc(target_tile->pieces,
				sizeof(piece_t *) * (target_tile->nb_piece + 1));
	if (!target_tile->pieces)
		exit(1);
	target_tile->pieces[target_tile->nb_piece++] = selected_piece;
	piece_t	*new_piece = target_tile->pieces[target_tile->nb_piece - 1];
	reset_possible_moves(board);
	if (!board->copy_board)
		return ;
	if (selected_piece->type == PAWN &&
		(y == (!selected_piece->color * (board->height - 1)))) {
		board->promo_tile = target_tile;
	}
	update_logs(board, new_piece, target_piece);
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
		printf("\r%*s%s%s", PROMO_OFFSET, "", CURSOR_RIGHT, CURSOR_RIGHT);
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
