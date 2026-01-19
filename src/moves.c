#include "cheese.h"

void	remove_piece(tile_t *target, int id, board_t *board, int is_moving, int attack)
{
	if (board->copy_board) {
		if (!is_moving) {
			target->pieces[id]->hp -= attack;
			if (target->pieces[id]->hp > 0)
				return ;
		}
		board->players[target->pieces[id]->color].nb_piece--;
	}
	else {
		piece_t	**new_pieces = malloc(sizeof(piece_t *) *
			target->nb_piece);
		memcpy(new_pieces, target->pieces, sizeof(piece_t *) * target->nb_piece);
		if (target->tile_type == COPY_TILE)
			free(target->pieces);
		target->tile_type = COPY_TILE;
		target->pieces = new_pieces;

		move_infos_t	*log = &board->simu_changes[board->simu_change_index++];
		log->piece = NULL;
		log->target_piece = target->pieces[id];
		log->origin_x = target->x;
		log->origin_y = target->y;
		log->target_x = -1;
		log->target_y = -1;
		if (!is_moving) {
			target->pieces[id]->simu_hp -= attack;
			if (target->pieces[id]->simu_hp > 0)
				return ;
			target->pieces[id]->simu_is_dead = 1;
		}
	}
	for (int i = (id + 1); i < target->nb_piece; i++) {
		target->pieces[i - 1] = target->pieces[i];
	}
	target->pieces[--target->nb_piece] = NULL;
}

piece_t	*simple_move(board_t *board, int y_src, int x_src, int y_dest, int x_dest) {
	tile_t	*origin_tile = &board->tiles[y_src][x_src];
	tile_t	*target_tile = &board->tiles[y_dest][x_dest];

	if (!board->copy_board) {
		piece_t	**real_pieces = target_tile->pieces;
		target_tile->pieces = malloc(sizeof(piece_t *) *
				(target_tile->nb_piece + 2));
		memcpy(target_tile->pieces, real_pieces, target_tile->nb_piece * sizeof(piece_t *));
		if (target_tile->tile_type == COPY_TILE)
			free(real_pieces);
		target_tile->tile_type = COPY_TILE;
	}
	else
		target_tile->pieces = realloc(target_tile->pieces,
			(target_tile->nb_piece + 2) * sizeof(piece_t *));
	if (!target_tile->pieces)
		exit(2);
	target_tile->pieces[target_tile->nb_piece++] = origin_tile->pieces[board->selector.origin_id];
	target_tile->pieces[target_tile->nb_piece] = NULL;
	remove_piece(origin_tile, board->selector.origin_id, board, 1, 0);
	if (origin_tile->tile_type == REAL_TILE)
		origin_tile->tile_type = MODIFIED_TILE;
	if (!board->copy_board) {
		move_infos_t	*log = &board->simu_changes[board->simu_change_index++];

		log->piece = target_tile->pieces[target_tile->nb_piece - 1];
		log->target_piece = NULL;
		log->origin_y = y_src;
		log->origin_x = x_src;
		log->target_y = y_dest;
		log->target_x = x_dest;
	}
	return (target_tile->pieces[target_tile->nb_piece - 1]);
}

void	move_piece(board_t *board, int y, int x)
{
	board->selector.target_y = y;
	board->selector.target_x = x;

	selector_t	*s = &board->selector;
	tile_t	*origin_tile = &board->tiles[s->origin_y][s->origin_x];
	tile_t	*target_tile = &board->tiles[s->target_y][s->target_x];
	piece_t *selected_piece = origin_tile->pieces[s->origin_id];
	piece_t	*target_piece = NULL;

	piece_t *new_piece = simple_move(board, s->origin_y, s->origin_x, s->target_y, s->target_x);
	if (target_tile->nb_piece &&
		selected_piece->color != target_tile->pieces[s->target_id]->color) {
		if (board->copy_board)
			target_piece = target_tile->pieces[s->target_id];
		remove_piece(target_tile, s->target_id, board, 0, selected_piece->attack_power);
	}
	else if (selected_piece->type == PAWN && (target_tile->nb_piece == 1) &&
		(s->target_x != s->origin_x) && !board->debug) {
		int y_pos = selected_piece->color == WHITE ? -1 : 1;
		if (board->copy_board)
			target_piece = board->tiles[y + y_pos][x].pieces[s->target_id];
		remove_piece(&board->tiles[y + y_pos][x], s->target_id, board, 0, 999);
	}
	else if (selected_piece->type == KING &&
		abs(s->target_x - s->origin_x) > 1) {
		int	x_vel = (s->target_x > s->origin_x) ? 1 : -1;
		int	x_target = s->origin_x;
		while (1) {
			x_target += x_vel;
			if (x_target < 0 || x_target >= board->width)
				exit(1);
			if (!board->tiles[s->origin_y][x_target].nb_piece)
				continue ;
			if (board->tiles[s->origin_y][x_target].pieces[0]->type == ROOK)
				break ;
		}
		piece_t	*rook = simple_move(board, s->origin_y, x_target, s->target_y, s->target_x - x_vel);
		if (board->copy_board)
			rook->x = s->target_x - x_vel;
	}
	reset_possible_moves(board);
	if (!board->copy_board)
		return ;
	if (selected_piece->type == PAWN &&
		(y == (!(selected_piece->color - 1) * (board->height - 1)))) {
		board->special_tile = target_tile;
	}
	update_logs(board, new_piece, target_piece);
}

void	highlight_board(board_t *board, int y, int x)
{
	int cursor_y;
	int	cursor_x;

	get_cursor_position(&cursor_x, &cursor_y);
	board->possible_locations = board->selected_piece->possible_locations;
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
			piece_t	*piece = get_tile_piece(board, j, i);
			if (piece && piece->color == BOARD)
				piece = NULL;
			if (board->possible_locations[j][i])
				printf("%s", BLUE_BG);
			else if (piece && piece->type == KING && piece->is_targeted)
				printf("%s", RED_BG);
			printf("%s%s", (piece != NULL) ? piece->character : " ", BLACK_BG);
			for (int k = 0; k < 3; k++)
				printf("%s", CURSOR_RIGHT);
		}
		for (int i = 0; i < 2; i++)
			printf("%s", CURSOR_DOWN);
	}
	printf("\033[%d;%dH\e[?25h", cursor_y, cursor_x);
	fflush(stdout);
}
