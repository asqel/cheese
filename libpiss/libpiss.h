#ifndef LIBPISS_H
#define LIBPISS_H

#include "../libcheese/libcheese_chess.h"

int	move_king(board_t *board, piece_t *target, int y, int x);
int	move_rook(board_t *board, piece_t *target, int y, int x);
int	move_bishop(board_t *board, piece_t *target, int y, int x);
int	move_knight(board_t *board, piece_t *target, int y, int x);
int	move_queen(board_t *board, piece_t *target, int y, int x);
int	move_pawn(board_t *board, piece_t *target, int y, int x);

#endif
