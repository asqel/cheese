#include "cheese.h"

void	evaluate_move(board_t *board, piece_t *target, int y, int x, int *valid_move)
{
	if (board->copy_board == NULL) {
		*valid_move = 1;
		board->possible_moves[y][x] = 1;
	}
	else {
		selector_t	*selec = &board->selector;
		board->copy_board->selector.origin_x = selec->origin_x;
		board->copy_board->selector.origin_y = selec->origin_y;
		board->copy_board->selector.origin_id = selec->origin_id;
		move_piece(board->copy_board, y, x);
		if (board->debug || !king_in_check(board, target->color)) {
			*valid_move = 1;
			board->possible_moves[y][x] = 1;
		}
		free(board->copy_board->tiles[y][x].pieces);
		board->copy_board->tiles[y][x] = board->tiles[y][x];
		board->copy_board->tiles[selec->origin_y][selec->origin_x] =
			board->tiles[selec->origin_y][selec->origin_x];
	}
}

int	move_pawn(board_t *board, piece_t *target, int y, int x)
{
	tile_t	*tile;
	int		valid_move = 0;
	int		vert_goal = target->color == WHITE ? 1 : -1;

	if ((vert_goal == -1 && y == 0) || (vert_goal == 1 && y == (board->height - 1)))
		return (0);
	for (int i = -1; i < 2; i++) {
		if ((x + i) < 0 || (x + i) >= board->width)
			continue ;
		tile = &board->tiles[y + vert_goal][x + i];
		if (!i && !tile->nb_piece) {
			evaluate_move(board, target, y + vert_goal, x, &valid_move);
			if (target->move_counter)
				continue ;
			if ((vert_goal == 1 && y == (board->height - 2)) || (vert_goal == -1 && y == 1))
				continue ;
			if (!board->tiles[y + vert_goal * 2][x].nb_piece)
				evaluate_move(board, target, y + vert_goal * 2, x, &valid_move);
		}
		else if (i && tile->nb_piece && (tile->pieces[0].color != target->color))
			evaluate_move(board, target, y + vert_goal, x + i, &valid_move);
		else if (i && !tile->nb_piece) {
			tile_t	*passant_tile = &board->tiles[y][x + i];
			if (!passant_tile->nb_piece)
				continue ;

			piece_t	*passant_piece = &passant_tile->pieces[0];
			if (passant_piece->color != target->color && passant_piece->type == PAWN)
			{
				y += 0;
				if (passant_piece->distance_moved != 1 && passant_piece->move_counter == 1)
					evaluate_move(board, target, y + vert_goal, x + i, &valid_move);
			}
		}
	}
	return (valid_move);
}

int	move_rook(board_t *board, piece_t *target, int y, int x)
{
	tile_t	*tile;
	int		valid_move = 0;
	int		y_moves[] = {-1, 1, 0, 0};
	int		x_moves[] = {0, 0, -1, 1};
	int		target_y, target_x;

	for (size_t i = 0; i < (sizeof(y_moves) / sizeof(y_moves[0])); i++) {
		target_y = y;
		target_x = x;
		while (1) {
			target_y += y_moves[i];
			target_x += x_moves[i];
			if (target_y < 0 || target_x < 0 ||
				target_y >= board->height || target_x >= board->width)
				break ;
			tile = &board->tiles[target_y][target_x];
			if (board->debug)
				evaluate_move(board, target, target_y, target_x, &valid_move);
			if (!board->debug && tile->nb_piece) {
				if (tile->pieces[0].color != target->color)
					evaluate_move(board, target, target_y, target_x, &valid_move);
				break ;
			}
			evaluate_move(board, target, target_y, target_x, &valid_move);
		}
	}
	return (valid_move);
}

int	move_knight(board_t *board, piece_t *target, int y, int x)
{
	tile_t	*tile;
	int		valid_move = 0;
	int		y_values[] = {-2, -2, 2, 2, -1, 1, -1, 1};
	int		x_values[] = {-1, 1, -1, 1, -2, -2, 2, 2};
	int		target_x, target_y;

	for (size_t i = 0; i < (sizeof(y_values) / sizeof(y_values[0])); i++) {
		target_y = y + y_values[i];
		target_x = x + x_values[i];
		if (target_y < 0 || target_x < 0 ||
			target_y >= board->height || target_x >= board->width)
			continue ;
		tile = &board->tiles[target_y][target_x];
		if (board->debug || !tile->nb_piece || tile->pieces[0].color != target->color)
			evaluate_move(board, target, target_y, target_x, &valid_move);
	}
	return (valid_move);
}

int	move_bishop(board_t *board, piece_t *target, int y, int x)
{
	tile_t	*tile;
	int		valid_move = 0;
	int		y_moves[] = {-1, -1, 1, 1};
	int		x_moves[] = {1, -1, -1, 1};
	int		target_x, target_y;

	for (size_t i = 0; i < (sizeof(y_moves) / sizeof(y_moves[0])); i++) {
		target_y = y;
		target_x = x;
		while (1) {
			target_y += y_moves[i];
			target_x += x_moves[i];
			if (target_y < 0 || target_x < 0 ||
				target_y >= board->height || target_x >= board->width)
				break ;
			tile = &board->tiles[target_y][target_x];
			if (board->debug)
				evaluate_move(board, target, target_y, target_x, &valid_move);
			if (!board->debug && tile->nb_piece) {
				if (tile->pieces[0].color != target->color)
					evaluate_move(board, target, target_y, target_x, &valid_move);
				break ;
			}
			evaluate_move(board, target, target_y, target_x, &valid_move);
		}
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
			if (board->debug || !tile->nb_piece || (tile->pieces[0].color != target->color))
				evaluate_move(board, target, target_y, target_x, &valid_move);
		}
	}
	return (valid_move);
}

int	update_possible_moves(board_t *board, int y, int x)
{
	board->selector.origin_x = x;
	board->selector.origin_y = y;
	if (board->copy_board)
		sync_boards(board->copy_board, board);
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
	return (1);
}
