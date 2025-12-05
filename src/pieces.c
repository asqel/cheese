#include "cheese.h"

piece_t	*create_piece(char piece, int index) {
	piece_t	*dest = malloc(sizeof(piece_t));
	if (!dest)
		exit(1);

	dest->piece_id = index;
	dest->color = WHITE;
	dest->is_dead = 0;
	dest->kill_count = 0;
	dest->move_counter = 0;
	dest->distance_moved = 0;
	if (piece >= 'a') {
		dest->color = BLACK;
		piece -= 32;
	}
	switch (piece) {
		case 'P':
			dest->type = PAWN;
			strcpy(dest->character, "♙");
			break ;
		case 'R':
			dest->type = ROOK;
			strcpy(dest->character, "♖");
			break ;
		case 'N':
			dest->type = KNIGHT;
			strcpy(dest->character, "♘");
			break ;
		case 'B':
			dest->type = BISHOP;
			strcpy(dest->character, "♗");
			break ;
		case 'Q':
			dest->type = QUEEN;
			strcpy(dest->character, "♕");
			break ;
		case 'K':
			dest->type = KING;
			strcpy(dest->character, "♔");
			break ;
	};
	if (dest->color == BLACK)
		dest->character[2] += "♚"[2] - "♔"[2];
	return (dest);
}

void	evaluate_move(board_t *board, piece_t *target, int y, int x, int *valid_move)
{
	if (board->copy_board == NULL) {
		*valid_move = 1;
		board->possible_moves[y][x] = 1;
	}
	else {
		int			in_check = board->players[target->color].king_in_check;
		selector_t	*selec = &board->selector;
		board->copy_board->selector.origin_x = selec->origin_x;
		board->copy_board->selector.origin_y = selec->origin_y;
		board->copy_board->selector.origin_id = selec->origin_id;
		move_piece(board->copy_board, y, x);
		if (board->debug || in_check || !king_in_check_simu(board, target->color)) {
			*valid_move = 1;
			target->possible_moves[y][x] = 1;
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
		else if (i && tile->nb_piece && (tile->pieces[0]->color != target->color))
			evaluate_move(board, target, y + vert_goal, x + i, &valid_move);
		else if (i && !tile->nb_piece) {
			tile_t	*passant_tile = &board->tiles[y][x + i];
			if (!passant_tile->nb_piece)
				continue ;

			piece_t	*passant_piece = passant_tile->pieces[0];
			if (passant_piece->color == target->color || passant_piece->type != PAWN ||
				passant_piece->move_counter != 1 || passant_piece->distance_moved != 2)
				continue ;

			move_infos_t	*log = board->logs->last_move;
			if (log->piece->piece_id == passant_piece->piece_id)
				evaluate_move(board, target, y + vert_goal, x + i, &valid_move);
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
				if (tile->pieces[0]->color != target->color)
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
		if (board->debug || !tile->nb_piece || tile->pieces[0]->color != target->color)
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
				if (tile->pieces[0]->color != target->color)
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
			if (board->debug || !tile->nb_piece || (tile->pieces[0]->color != target->color))
				evaluate_move(board, target, target_y, target_x, &valid_move);
		}
	}
	return (valid_move);
}

/* y < 0 && x < 0 = all pieces
   y < 0 && x >= 0 = only x color
   y > 0 && x <= 0 = all but y color
*/
int	update_possible_moves(board_t *board, int y, int x)
{
	if (y >= 0 && x >= 0) {
		board->selector.origin_x = x;
		board->selector.origin_y = y;
		piece_t	*piece = board->tiles[y][x].pieces[board->selector.origin_id];
		board->possible_moves = piece->possible_moves;
		return (piece->can_move);
	}
	sync_boards(board->copy_board, board);
	for (int j = 0; j < board->height; j++) {
		for (int i = 0; i < board->width; i++) {
			for (int k = 0; k < board->tiles[j][i].nb_piece; k++) {
				piece_t	*piece = board->tiles[j][i].pieces[k];
				if (x < 0 && y < 0)
					for (int l = 0; l < board->height; l++)
						memset(piece->possible_moves[l], 0, board->width);
				if (piece->is_dead ||
					(x >= 0 && piece->color != x) ||
					(y >= 0 && piece->color == x))
					continue ;
				piece->can_move = simulate_piece(board, piece) != 0;
			}
		}
	}
	return (1);
}

int	simulate_piece(board_t *board, piece_t *target) {
	board->selector.origin_x = target->x;
	board->selector.origin_y = target->y;
	switch (target->type) {
		case PAWN:
			return (move_pawn(board, target, target->y, target->x));
		case ROOK:
			return (move_rook(board, target, target->y, target->x));
		case KNIGHT:
			return (move_knight(board, target, target->y, target->x));
		case BISHOP:
			return (move_bishop(board, target, target->y, target->x));
		case QUEEN:
			return (move_rook(board, target, target->y, target->x) + move_bishop(board, target, target->y, target->x));
		case KING:
			return (move_king(board, target, target->y, target->x));
	}
	return (1);
}
