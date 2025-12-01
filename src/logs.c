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

void	update_logs(board_t *board, piece_t *piece, piece_t *target) {
	int				piece_color = piece->color;
	move_logs_t		*logs = board->logs;
	move_infos_t	*move = &logs->color_logs[piece_color][logs->color_moves[piece_color]];

	update_move_counter(&board->selector, piece);
	logs->last_color_played = piece_color;
	piece->move_counter++;
	if (target)
		piece->kill_count++;
	move->piece = piece;
	move->color = piece_color;
	move->target_piece = target;
	move->origin_x = board->selector.origin_x;
	move->origin_y = board->selector.origin_y;
	move->target_x = board->selector.target_x;
	move->target_y = board->selector.target_y;
	logs->last_move = &logs->color_logs[piece_color][logs->color_moves[piece_color]++];
	logs->global_log[logs->nb_move++] = logs->last_move;
	if (logs->nb_move >= MAX_LOG) {
		fprintf(stderr, "YOUR TAKING TOO LONG\n");
		exit(1);
	}
}
