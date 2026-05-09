#include "libcheese_chess.h"

void	remove_piece(tile_t *target, int id, board_t *board)
{
	board->players[target->pieces[id]->type->color].nb_piece--;
	for (int i = (id + 1); i < target->nb_piece; i++) {
		--target->pieces[i]->tile_id;
		target->pieces[i - 1] = target->pieces[i];
	}
	target->pieces[--target->nb_piece] = NULL;
}

void	damage_piece(board_t *board, tile_t *target_tile,
		piece_t *victim, piece_t *attacker, int attack)
{
	victim->hp -= attack;
	if (victim->hp > 0) {
		if (attacker->type->default_attack_func)
			attacker->type->default_hurt_func(board, attacker, victim);
		if (victim->type->default_hurt_func)
			victim->type->default_hurt_func(board, victim, attacker);
		return ;
	}
	remove_piece(target_tile, victim->tile_id, board);
	if (attacker->type->default_kill_func)
		attacker->type->default_kill_func(board, attacker, victim);
	if (victim->type->default_death_func)
		victim->type->default_death_func(board, victim, attacker);
}

piece_t	*simple_move(board_t *board, piece_t *target_piece, int y_dest, int x_dest)
{
	tile_t	*origin_tile = target_piece->cur_tile;
	tile_t	*target_tile = &board->tiles[y_dest][x_dest];

	target_tile->pieces = realloc(target_tile->pieces,
		(target_tile->nb_piece + 2) * sizeof(piece_t *));
	if (!target_tile->pieces)
		exit(2);

	target_piece->x = x_dest;
	target_piece->y = y_dest;
	target_piece->tile_id = target_tile->nb_piece;
	target_piece->cur_tile = target_tile;
	target_tile->pieces[target_tile->nb_piece++] = origin_tile->pieces[board->selector.origin_id];
	target_tile->pieces[target_tile->nb_piece] = NULL;
	remove_piece(origin_tile, board->selector.origin_id, board);
	return (target_tile->pieces[target_tile->nb_piece - 1]);
}

void	default_move_piece(board_t *board)
{
	selector_t	*s = &board->selector;
	tile_t	*origin_tile = &board->tiles[s->origin_y][s->origin_x];
	tile_t	*target_tile = &board->tiles[s->target_y][s->target_x];
	piece_t *selected_piece = origin_tile->pieces[s->origin_id];
	piece_t	*target_piece = NULL;

	piece_t *new_piece = simple_move(board, selected_piece, s->target_y, s->target_x);
	if ((target_tile->nb_piece > 1) && (selected_piece->type->is_cannibal || 
		selected_piece->type->color != target_tile->pieces[s->target_id]->type->color)) {
		target_piece = target_tile->pieces[s->target_id];
		damage_piece(board, target_tile, target_piece, new_piece, new_piece->attack_power);
	}
	reset_possible_moves(board);
	update_logs(board, new_piece, target_piece);
}
