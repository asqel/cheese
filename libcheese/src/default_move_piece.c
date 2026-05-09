#include "libcheese_chess.h"

void	remove_piece(tile_t *target, int id, board_t *board, int is_moving, int attack)
{
	if (!is_moving) {
		target->pieces[id]->hp -= attack;
		if (target->pieces[id]->hp > 0)
			return ;
	}
	board->players[target->pieces[id]->type->color].nb_piece--;
	for (int i = (id + 1); i < target->nb_piece; i++) {
		target->pieces[i - 1] = target->pieces[i];
	}
	target->pieces[--target->nb_piece] = NULL;
}

piece_t	*simple_move(board_t *board, int y_src, int x_src, int y_dest, int x_dest)
{
	tile_t	*origin_tile = &board->tiles[y_src][x_src];
	tile_t	*target_tile = &board->tiles[y_dest][x_dest];

	target_tile->pieces = realloc(target_tile->pieces,
		(target_tile->nb_piece + 2) * sizeof(piece_t *));
	if (!target_tile->pieces)
		exit(2);
	target_tile->pieces[target_tile->nb_piece++] = origin_tile->pieces[board->selector.origin_id];
	target_tile->pieces[target_tile->nb_piece] = NULL;
	remove_piece(origin_tile, board->selector.origin_id, board, 1, 0);
	if (origin_tile->tile_type == REAL_TILE)
		origin_tile->tile_type = MODIFIED_TILE;
	return (target_tile->pieces[target_tile->nb_piece - 1]);
}

void	default_move_piece(board_t *board, int y, int x)
{
	board->selector.target_y = y;
	board->selector.target_x = x;

	selector_t	*s = &board->selector;
	tile_t	*origin_tile = &board->tiles[s->origin_y][s->origin_x];
	tile_t	*target_tile = &board->tiles[s->target_y][s->target_x];
	piece_t *selected_piece = origin_tile->pieces[s->origin_id];
	piece_t	*target_piece = NULL;

	piece_t *new_piece = simple_move(board, s->origin_y, s->origin_x, s->target_y, s->target_x);
	if ((target_tile->nb_piece > 1) && (selected_piece->type->is_cannibal || 
		selected_piece->type->color != target_tile->pieces[s->target_id]->type->color)) {
		target_piece = target_tile->pieces[s->target_id];
		remove_piece(target_tile, s->target_id, board, 0, selected_piece->attack_power);
	}
	reset_possible_moves(board);
	update_logs(board, new_piece, target_piece);
}
