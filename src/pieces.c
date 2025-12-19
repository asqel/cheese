#include "cheese.h"

void	reset_simulation(board_t *board) {
	for (int i = 0; i < board->copy_board->simu_change_index; i++) {
		move_infos_t	*change = &board->copy_board->simu_changes[i];
		if (!change->piece)
			continue ;

		piece_t	***pieces = &board->copy_board->tiles[change->target_y][change->target_x].pieces;
		if (*pieces != NULL)
				free(*pieces);
		*pieces = NULL;

		pieces = &board->copy_board->tiles[change->origin_y][change->origin_x].pieces;
		if (*pieces != NULL)
			free(*pieces);
		*pieces = NULL;
	}
	for (int i = 0; i < board->copy_board->simu_change_index; i++) {
		move_infos_t	*change = &board->copy_board->simu_changes[i];

		if (!change->piece && change->target_piece) {
			change->target_piece->simu_is_dead = 0;
			change->target_piece->simu_hp = change->target_piece->hp;
		}
		board->copy_board->tiles[change->origin_y][change->origin_x] =
			board->tiles[change->origin_y][change->origin_x];
		if (change->target_y < 0 || change->target_x < 0)
			continue ;
		board->copy_board->tiles[change->target_y][change->target_x] =
			board->tiles[change->target_y][change->target_x];
	}
}

void	evaluate_move(board_t *board, piece_t *target, int y, int x, int *valid_move) {
	tile_t		*target_tile = &board->tiles[y][x];

	if (!board->players[target->color].compute_check) {
		*valid_move = 1;
		target->possible_locations[y][x] = 1;
		for (int p = 0; p < target_tile->nb_piece; p++)
			if (target_tile->pieces[p]->color != target->color)
				target->possible_moves[y][x][p] += target->attack_power;
		if (!target_tile->nb_piece)
			target->possible_moves[y][x][0] += target->attack_power;
		return ;
	}
	if (board->copy_board == NULL) {
		*valid_move = 1;
		for (int p = 0; p < max(1, target_tile->nb_piece); p++)
			if (target_tile->pieces[p]->color != target->color)
				target->copy_moves[y][x][p] += target->attack_power;
		return ;
	}

	int			is_checkmate = board->players[target->color].king_in_check == 2;
	for (int p = 0; p < max(1, target_tile->nb_piece); p++) {
		int	compute_move = board->debug;
		if (target_tile->nb_piece < 2)
			compute_move = 1;
		else if (target_tile->pieces[p]->color != target->color)
			compute_move = 1;
		if (!compute_move)
			continue ;
		board->copy_board->selector = board->selector;
		board->copy_board->selector.target_id = p;
		board->copy_board->simu_change_index = 0;
		if (!board->debug)
			move_piece(board->copy_board, y, x);
		if (board->debug || is_checkmate || !king_in_check_simu(board, target->color)) {
			*valid_move = 1;
			target->possible_moves[y][x][p] += target->attack_power;
			target->possible_locations[y][x] = 1;
		}
		reset_simulation(board);
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
		else if (i && tile->nb_piece && get_nb_pieces_on_tile(tile, -target->color))
			evaluate_move(board, target, y + vert_goal, x + i, &valid_move);
		else if (i && !tile->nb_piece) {
			tile_t	*passant_tile = &board->tiles[y][x + i];
			if (!passant_tile->nb_piece)
				continue ;

			piece_t	*passant_piece = passant_tile->pieces[0];
			if (passant_piece->color == target->color || passant_piece->type != target->type ||
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

	if (get_nb_pieces_on_tile(&board->tiles[y][x], -target->color))
		evaluate_move(board, target, y, x, &valid_move);
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
			else if (tile->nb_piece) {
				if (get_nb_pieces_on_tile(tile, -target->color))
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
		if (board->debug || !tile->nb_piece || get_nb_pieces_on_tile(tile, -target->color))
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
				if (get_nb_pieces_on_tile(tile, -target->color))
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

	if (get_nb_pieces_on_tile(&board->tiles[y][x], -target->color))
		evaluate_move(board, target, y, x, &valid_move);
	for (int y_offset = -1; y_offset < 2; y_offset++) {
		int	target_y = y + y_offset;
		if (target_y < 0 || target_y >= board->height)
			continue ;
		for (int x_offset = -1; x_offset < 2; x_offset++) {
			int	target_x = x + x_offset;
			if (target_x < 0 || target_x >= board->width)
				continue ;
			if (target_x == x && target_y == y)
				continue ;
			tile = &board->tiles[target_y][target_x];
			if (board->debug || !tile->nb_piece || get_nb_pieces_on_tile(tile, -target->color))
				evaluate_move(board, target, target_y, target_x, &valid_move);
			if (x_offset == 0 || y_offset != 0 || target->move_counter || target->is_targeted)
				continue ;
			int	castling_x = x;
			while (1) {
				castling_x += x_offset;
				if (castling_x < 0 || castling_x >= board->width)
					break ;
				tile = &board->tiles[target_y][castling_x];
				if (!tile->nb_piece)
					continue ;
				else if (tile->nb_piece != 1)
					break ;
				if ((x + (x_offset * 2) < 0) || (x + (x_offset * 2) >= board->width))
					break ;
				piece_t	*piece = tile->pieces[0];
				if (piece->type == ROOK && piece->move_counter == 0 &&
					piece->color == target->color) {
					evaluate_move(board, target, target_y, x + (x_offset * 2), &valid_move);
				}
				break ;
			}
		}
	}
	return (valid_move);
}

/* y < 0 && x < 0 = all pieces
   y < 0 && x >= 0 = only x color
   y > 0 && x <= 0 = all but y color
*/
int	update_possible_moves(board_t *board, int y, int x) {
	if (y >= 0 && x >= 0) {
		board->selector.origin_x = x;
		board->selector.origin_y = y;
		piece_t	*piece = board->tiles[y][x].pieces[board->selector.origin_id];
		board->selected_piece = piece;
		board->possible_moves = piece->possible_moves;
		board->possible_locations = piece->possible_locations;
		return (piece->can_move);
	}
	sync_boards(board->copy_board, board);
	for (int p = 0; p < board->nb_piece; p++) {
		piece_t	*piece = board->pieces[p];
		if (piece->is_dead ||
			(x >= 0 && piece->color != x) ||
			(y >= 0 && piece->color == x))
			continue ;
		piece->can_move = simulate_piece(board, piece) != 0;
		x++;
		x--;
	}
	return (1);
}

int	simulate_piece(board_t *board, piece_t *target) {
	board->selector.origin_x = target->x;
	board->selector.origin_y = target->y;
	board->selector.origin_id = target->tile_id;
	if (board->copy_board) {
		for (int j = 0; j < board->height; j++) {
			memset(target->possible_locations[j], 0, board->width);
			for (int i = 0; i < board->width; i++)
				memset(target->possible_moves[j][i], 0, max(1, board->tiles[j][i].nb_piece));
		}
	}
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
