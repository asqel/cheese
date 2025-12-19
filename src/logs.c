#include "cheese.h"

void	update_move_counter(selector_t *selector, piece_t *piece) {
	switch (piece->type) {
		case KING:
		case KNIGHT:
			piece->distance_moved++;
			break ;
		case ROOK:
			piece->distance_moved +=
				abs(selector->origin_x - selector->target_x) +
				abs(selector->origin_y - selector->target_y);
			break ;
		case PAWN:
		case BISHOP:
			piece->distance_moved +=
				abs(selector->origin_y - selector->target_y);
			break ;
		case QUEEN:
			piece->distance_moved += abs(selector->origin_y - selector->target_y);
			if ((selector->origin_y == selector->target_y) ||
				(selector->origin_x == selector->target_x))
				piece->distance_moved += abs(selector->origin_x - selector->target_x);
			break ;
	}
}

void	update_tile_ids(tile_t **tiles, selector_t *selec) {
	tile_t	*origin = &tiles[selec->origin_y][selec->origin_x];
	tile_t	*target = &tiles[selec->target_y][selec->target_x];

	for (int id = 0; id < origin->nb_piece; id++)
		origin->pieces[id]->tile_id = id;
	for (int id = 0; id < target->nb_piece; id++)
		target->pieces[id]->tile_id = id;
}

void	update_logs(board_t *board, piece_t *piece, piece_t *target) {
	int				piece_color = piece->color;
	move_logs_t		*logs = board->logs;
	move_infos_t	*move = &logs->color_logs[piece_color][logs->color_moves[piece_color]];

	update_move_counter(&board->selector, piece);
	logs->last_color_played = piece_color;
	piece->move_counter++;
	if (target && target->hp <= 0) {
		if (target->type == KING)
			board->players[target->color].nb_kings--;
		piece->kill_count++;
		target->is_dead = 1;
	}
	move->piece = piece;
	move->color = piece_color;
	move->target_piece = target;
	move->origin_x = board->selector.origin_x;
	move->origin_y = board->selector.origin_y;
	move->target_x = board->selector.target_x;
	move->target_y = board->selector.target_y;
	update_tile_ids(board->tiles, &board->selector);
	logs->last_move = &logs->color_logs[piece_color][logs->color_moves[piece_color]++];
	logs->global_log[logs->nb_move++] = logs->last_move;
	/*if (piece->x != move->origin_x || piece->y != move->origin_y) {
		fprintf(stderr, "\e[?1049lMismatched position\n");
		exit(1);
	}*/
	piece->x = move->target_x;
	piece->y = move->target_y;
	piece->tile_id = board->tiles[piece->y][piece->x].nb_piece - 1;
	if (logs->nb_move >= MAX_LOG) {
		fprintf(stderr, "\e[?1049lYOUR TAKING TOO LONG\n");
		exit(1);
	}
}
