#include "cheese.h"

int	move_pawn(board_t *board, piece_t *target, int y, int x)
{
	tile_t	*tile;
	int		valid_move = 0;
	int		vert_goal = 1;

	if (target->color == BLACK)
		vert_goal = -1;
	if ((vert_goal == -1 && y == 0) || (vert_goal == 1 && y == (board->height - 1)))
		return (0);
	for (int i = -1; i < 2; i++) {
		tile = &board->tiles[y + vert_goal][x + i];
		if (!i && !tile->nb_piece) {
			board->possible_moves[y + vert_goal][x] = 1;
			valid_move = 1;
			if (target->move_counter)
				continue ;
			if ((vert_goal == 1 && y == (board->height - 2)) || (vert_goal == -1 && y == 1))
				continue ;
			if (!board->tiles[y + vert_goal * 2][x].nb_piece)
				board->possible_moves[y + vert_goal * 2][x] = 1;
		}
		else if (i && (tile->nb_piece && tile->pieces[0].color != target->color)) {
			board->possible_moves[y + vert_goal][x + i] = 1;
			valid_move = 1;
		}
	}
	return (valid_move);
}

int	move_rook(board_t *board, piece_t *target, int y, int x)
{
	tile_t	*tile;
	int		valid_move = 0;
	int		target_y = y;
	int		target_x = x;

	while (--target_y >= 0) {
		tile = &board->tiles[target_y][x];
		if (tile->nb_piece) {
			if (tile->pieces[0].color != target->color) {
				board->possible_moves[target_y][x] = 1;
				valid_move = 1;
			}
			break ;
		}
		board->possible_moves[target_y][x] = 1;
		valid_move = 1;
	}
	target_y = y;
	while (++target_y < board->height) {
		tile = &board->tiles[target_y][x];
		if (tile->nb_piece) {
			if (tile->pieces[0].color != target->color) {
				board->possible_moves[target_y][x] = 1;
				valid_move = 1;
			}
			break ;
		}
		board->possible_moves[target_y][x] = 1;
		valid_move = 1;
	}
	while (++target_x < board->width) {
		tile = &board->tiles[y][target_x];
		if (tile->nb_piece) {
			if (tile->pieces[0].color != target->color) {
				board->possible_moves[y][target_x] = 1;
				valid_move = 1;
			}
			break ;
		}
		board->possible_moves[y][target_x] = 1;
		valid_move = 1;
	}
	target_x = x;
	while (--target_x >= 0) {
		if (board->tiles[y][target_x].nb_piece) {
			if (board->tiles[y][target_x].pieces[0].color != target->color) {
				board->possible_moves[y][target_x] = 1;
				valid_move = 1;
			}
			break ;
		}
		board->possible_moves[y][target_x] = 1;
		valid_move = 1;
	}
	return (valid_move);
}

int	move_knight(board_t *board, piece_t *target, int y, int x)
{
	tile_t	*tile;
	int		valid_move = 0;

	if (y > 1) {
		tile = &board->tiles[y - 2][x - 1];
		if (x && (!tile->nb_piece || tile->pieces[0].color != target->color)) {
			board->possible_moves[y - 2][x - 1] = 1;
			valid_move = 1;
		}
		tile = &board->tiles[y - 2][x + 1];
		if ((x < (board->width - 1)) && (!tile->nb_piece || tile->pieces[0].color != target->color)) {
			board->possible_moves[y - 2][x + 1] = 1;
			valid_move = 1;
		}
	}
	if (y < (board->height - 2)) {
		tile = &board->tiles[y + 2][x - 1];
		if (x && (!tile->nb_piece || tile->pieces[0].color != target->color)) {
			board->possible_moves[y + 2][x - 1] = 1;
			valid_move = 1;
		}
		tile = &board->tiles[y + 2][x + 1];
		if ((x < (board->width - 1)) && (!tile->nb_piece || tile->pieces[0].color != target->color)) {
			board->possible_moves[y + 2][x + 1] = 1;
			valid_move = 1;
		}
	}
	if (x > 1) {
		tile = &board->tiles[y - 1][x - 2];
		if (y && (!tile->nb_piece || tile->pieces[0].color != target->color)) {
			board->possible_moves[y - 1][x - 2] = 1;
			valid_move = 1;
		}
		tile = &board->tiles[y + 1][x - 2];
		if ((y < (board->height - 1)) && (!tile->nb_piece || tile->pieces[0].color != target->color)) {
			board->possible_moves[y + 1][x - 2] = 1;
			valid_move = 1;
		}
	}
	if (x < (board->width - 2)) {
		tile = &board->tiles[y - 1][x + 2];
		if (y && (!tile->nb_piece || tile->pieces[0].color != target->color)) {
			board->possible_moves[y - 1][x + 2] = 1;
			valid_move = 1;
		}
		tile = &board->tiles[y + 1][x + 2];
		if ((y < (board->height - 1)) && (!tile->nb_piece || tile->pieces[0].color != target->color)) {
			board->possible_moves[y + 1][x + 2] = 1;
			valid_move = 1;
		}
	}
	return (valid_move);
}

int	move_bishop(board_t *board, piece_t *target, int y, int x)
{
	tile_t	*tile;
	int		valid_move = 0;
	int		target_y = y;
	int		target_x = x;

	while (--target_y >= 0 && --target_x >= 0) {
		tile = &board->tiles[target_y][target_x];
		if (tile->nb_piece) {
			if (tile->pieces[0].color != target->color) {
				board->possible_moves[target_y][target_x] = 1;
				valid_move = 1;
			}
			break ;
		}
		board->possible_moves[target_y][target_x] = 1;
		valid_move = 1;
	}
	target_y = y;
	target_x = x;
	while (++target_y < board->height && ++target_x < board->width) {
		tile = &board->tiles[target_y][target_x];
		if (tile->nb_piece) {
			if (tile->pieces[0].color != target->color) {
				board->possible_moves[target_y][target_x] = 1;
				valid_move = 1;
			}
			break ;
		}
		board->possible_moves[target_y][target_x] = 1;
		valid_move = 1;
	}
	target_y = y;
	target_x = x;
	while (++target_y < board->height && --target_x >= 0) {
		tile = &board->tiles[target_y][target_x];
		if (tile->nb_piece) {
			if (tile->pieces[0].color != target->color) {
				board->possible_moves[target_y][target_x] = 1;
				valid_move = 1;
			}
			break ;
		}
		board->possible_moves[target_y][target_x] = 1;
		valid_move = 1;
	}
	target_y = y;
	target_x = x;
	while (--target_y >= 0 && ++target_x < board->width) {
		tile = &board->tiles[target_y][target_x];
		if (tile->nb_piece) {
			if (tile->pieces[0].color != target->color) {
				board->possible_moves[target_y][target_x] = 1;
				valid_move = 1;
			}
			break ;
		}
		board->possible_moves[target_y][target_x] = 1;
		valid_move = 1;
	}
	return (valid_move);
}

int	move_king(board_t *board, piece_t *target, int y, int x)
{
	tile_t	*tile;
	int		valid_move = 0;

	for (int target_y = (y - 1); target_y < (y + 2); target_y++) {
		if (target_y < 0 || target_y >= board->height)
			continue ;
		for (int target_x = (x - 1); target_x < (x + 2); target_x++) {
			if (target_x < 0 || target_x >= board->width)
				continue ;
			if (target_x == x && target_y == y)
				continue ;
			tile = &board->tiles[target_y][target_x];
			if (!tile->nb_piece || (tile->pieces[0].color != target->color)) {
				board->possible_moves[target_y][target_x] = 1;
				valid_move = 1;
			}
		}
	}
	return (valid_move);
}

int	update_possible_moves(board_t *board, int y, int x)
{
	reset_possible_moves(board);
	board->selector.origin_x = x;
	board->selector.origin_y = y;
	board->selector.origin_id = 0;
	piece_t	*target = &board->tiles[y][x].pieces[board->selector.origin_id];

	if (target->type == PAWN)
		return (move_pawn(board, target, y, x));
	else if (target->type == ROOK)
		return (move_rook(board, target, y, x));
	else if (target->type == KNIGHT)
		return (move_knight(board, target, y, x));
	else if (target->type == BISHOP)
		return (move_bishop(board, target, y, x));
	else if (target->type == QUEEN)
		return (move_rook(board, target, y, x) + move_bishop(board, target, y, x));
	else if (target->type == KING)
		return (move_king(board, target, y, x));
	else {
		if (y > 1) {
			board->possible_moves[y - 1][x] = 1;
			board->possible_moves[y - 2][x] = 1;
		}
	}
	return (1);
}
