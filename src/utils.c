#include "cheese.h"
#include "oeuf.h"

int	max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

int	min(int a, int b)
{
	if (a < b)
		return (a);
	return (b);
}

void	print_error(char *error, int ret) {
	printf("\e[?1049l%s\n", error);
	exit(ret);
}

char	*strjoin(char *s1, char *s2, int free_s1)
{
	char	*dest;

	if (!s1)
		return (strdup(s2));
	if (!s2)
		return (s1);

	size_t s1_len = strlen(s1);
	size_t s2_len = strlen(s2);
	dest = calloc((s1_len + s2_len + 1), sizeof(char));
	if (!dest)
		return (NULL);
	strcat(dest, s1);
	strcat(dest + s1_len, s2);
	dest[s1_len + s2_len] = 0;
	if (free_s1)
		free(s1);
	return (dest);
}

void	get_cursor_position(int *x, int *y)
{
	char	buffer[16];

	enable_raw_mode();
	tcflush(0, TCIFLUSH);
	write(1, "\033[6n", 4);
	int readd = read(0, buffer, sizeof(buffer));
	disable_raw_mode();
	if (readd <= 3 || readd >= (int)sizeof(buffer)) {
		fprintf(stderr, "Problem while reading stdin\n");
		exit(1);
	}
	buffer[readd] = 0;
	int offset = 0;
	while (buffer[offset] < '0' || buffer[offset] > '9')
		offset++;

	*y = atoi(buffer + offset);
	*x = atoi(strchr(buffer, ';') + 1);
}

piece_t	*get_tile_piece(board_t *board, int y, int x)
{
	if (board->tiles[y][x].nb_piece)
		return (board->tiles[y][x].pieces[0]);
	return (NULL);
}

void	reset_possible_moves(board_t *board)
{
	for (int j = 0; j < board->height; j++)
		for (int i = 0; i < board->width; i++)
			memset(board->default_moves[j][i], 0, board->nb_piece);
	board->possible_moves = board->default_moves;
}

void	free_possible_moves(board_t *board, char ***moves) {
	for (int j = 0; j < board->height; j++) {
		for (int i = 0; i < board->width; i++)
			free(moves[j][i]);
		free(moves[j]);
	}
	free(moves);
}

void	free_board(board_t *board)
{
	for (int j = 0; j < board->height; j++) {
		if (board->copy_board) {
			for (int i = 0; i < board->width; i++) {
				if (board->tiles[j][i].pieces)
					free(board->tiles[j][i].pieces);
			}
		}
		free(board->tiles[j]);
	}
	if (board->copy_board)
		oe_strarr_free(board->occupied_map, board->height);
	else
		free(board->simu_changes);
	free(board->tiles);
}

int	get_nb_pieces_on_tile(tile_t *tile, int color)
{
	int	res = 0;

	for (int i = 0; i < tile->nb_piece; i++) {
		if (color > 0 && tile->pieces[i]->color == color)
			res++;
		else if (color < 0 && tile->pieces[i]->color != color)
			res++;
	}
	return (res);
}
