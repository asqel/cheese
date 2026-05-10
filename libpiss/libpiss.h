#ifndef LIBPISS_H
#define LIBPISS_H

#include "../libcheese/libcheese_chess.h"

void default_evaluate_move(board_t *board, piece_t *target, int y, int x, int *valid_move);
void default_move_piece(board_t *board);
void attack_no_move(board_t *board);

int target_all(board_t *board, piece_t *target, int y, int x);
int	move_king(board_t *board, piece_t *target, int y, int x);
int	move_rook(board_t *board, piece_t *target, int y, int x);
int	move_bishop(board_t *board, piece_t *target, int y, int x);
int	move_knight(board_t *board, piece_t *target, int y, int x);
int	move_queen(board_t *board, piece_t *target, int y, int x);
int	move_pawn(board_t *board, piece_t *target, int y, int x);

void im_hurt(board_t *board, piece_t *myself, piece_t *attacker);
void im_dead(board_t *board, piece_t *myself, piece_t *attacker);
void atomic_death(board_t *board, piece_t *myself, piece_t *attacker);
void atomic_death_recursive(board_t *board, piece_t *myself, piece_t *attacker);

#endif
