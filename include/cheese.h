#ifndef CHEESE_H
#define CHEESE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

#define CURSOR_UP "\033[A"
#define CURSOR_DOWN "\033[B"
#define CURSOR_RIGHT "\033[C"
#define CURSOR_LEFT "\033[D"

#define WHITE	0
#define BLACK	1

enum pieces {
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING
};

typedef struct {
	int		type;
	int		color;
	char	*character;
}	piece_t;

typedef struct {
	int		color;
	int		nb_piece;
	piece_t	*pieces;
}	tile_t;

typedef struct {
	int		width;
	int		height;
	int		white_pieces;
	int		black_pieces;
	tile_t	**tiles;
}	board_t;

void	init_board(char *filepath, board_t *board);

//utils
char	*strjoin(char *s1, char *s2, int free_s1);
int		max(int a, int b);
int		min(int a, int b);

//buffers
typedef struct {
	void *data;
	size_t len;
	size_t alloc_len;
} buffer_t;

int		buffer_append(buffer_t *buffer, void *data, size_t len);
void	buffer_remove(buffer_t *buffer, size_t start, size_t len);
void 	buffer_free(buffer_t *buffer);

#endif
