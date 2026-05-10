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

int damage_piece(board_t *board, tile_t *target_tile,
		piece_t *victim, piece_t *attacker, int attack)
{
	victim->hp -= attack;
	if (victim->hp > 0) {
		call_piece_callback(board, ON_ATTACK, attacker, victim);
		call_piece_callback(board, ON_HIT, attacker, victim);
		return (0);
	}
	if (attacker)
		++attacker->kill_count;
	remove_piece(target_tile, victim->tile_id, board);
	victim->is_dead = 1;
	if (victim->type->is_king)
		--board->players[victim->type->color].nb_kings;

	call_piece_callback(board, ON_KILL, attacker, victim);
	call_piece_callback(board, ON_DEATH, victim, attacker);
	return (1);
}

piece_t	*simple_move(board_t *board, piece_t *target_piece, int y_dest, int x_dest)
{
	tile_t	*origin_tile = target_piece->cur_tile;
	tile_t	*target_tile = &board->tiles[y_dest][x_dest];

	if (origin_tile == target_tile)
		return (target_piece);

	target_piece->move_counter++;
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
