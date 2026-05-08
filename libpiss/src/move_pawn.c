#include "libpiss.h"

int	move_pawn(board_t *board, piece_t *target, int y, int x)
{
	tile_t	*tile;
	int		valid_move = 0;
	int		vert_goal = target->type->color == WHITE ? 1 : -1;

	if ((vert_goal == -1 && y == 0) || (vert_goal == 1 && y == (board->height - 1)))
		return (0);
	for (int i = -1; i < 2; i++) {
		if ((x + i) < 0 || (x + i) >= board->width)
			continue ;
		tile = &board->tiles[y + vert_goal][x + i];
		if (!i && !tile->nb_piece) {
			default_evaluate_move(board, target, y + vert_goal, x, &valid_move);
			if (target->move_counter)
				continue ;
			if ((vert_goal == 1 && y == (board->height - 2)) || (vert_goal == -1 && y == 1))
				continue ;
			if (!board->tiles[y + vert_goal * 2][x].nb_piece)
				default_evaluate_move(board, target, y + vert_goal * 2, x, &valid_move);
		}
		else if (i && tile->nb_piece && get_nb_pieces_on_tile(tile, -target->type->color))
			default_evaluate_move(board, target, y + vert_goal, x + i, &valid_move);
		else if (i && !tile->nb_piece) {
			tile_t	*passant_tile = &board->tiles[y][x + i];
			if (!passant_tile->nb_piece)
				continue ;

			piece_t	*passant_piece = passant_tile->pieces[0];
			if (passant_piece->type->color == target->type->color || passant_piece->type != target->type ||
				passant_piece->move_counter != 1)
				continue ;

			move_infos_t	*log = board->logs->last_move;
			if (log->piece->piece_id == passant_piece->piece_id)
				default_evaluate_move(board, target, y + vert_goal, x + i, &valid_move);
		}
	}
	return (valid_move);
}
