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

char	*strjoin(char *src, char *to_add)
{
	char	*dest;

	if (!src)
		return (strdup(to_add));
	if (!to_add)
		return (src);

	size_t src_len = strlen(src);
	size_t to_add_len = strlen(to_add);
	dest = realloc(src, (src_len + to_add_len + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	memcpy(dest + src_len, to_add, to_add_len);
	dest[src_len + to_add_len] = 0;
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
	for (int j = 0; j < board->height; j++) {
		memset(board->default_locations[j], 0, board->width);
		for (int i = 0; i < board->width; i++)
			memset(board->default_moves[j][i], 0, board->nb_piece);
	}
	board->possible_moves = board->default_moves;
	board->possible_locations = board->default_locations;
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
	int must_match = (color > 0);

	if (color == BOARD)
		return (0);
	for (int i = 0; i < tile->nb_piece; i++) {
		int piece_color = tile->pieces[i]->color;
		if (must_match && piece_color == color)
			res++;
		else if (!must_match && piece_color != -color && piece_color != BOARD)
			res++;
		else if (!must_match && piece_color == BOARD && !tile->pieces[i]->invincible)
			res++;
	}
	return (res);
}

size_t strlen_utf8(char *str_ptr) {
	uint8_t *str = (uint8_t *)str_ptr;
	size_t res = 0;
	size_t i = 0;
	size_t len = strlen(str_ptr);
	while (i < len) {
		if (str[i] <= 0b01111111)
			i++;
		else if (str[i] >> 5 == 0b110)
			i += 2;
		else if (str[i] >> 4 == 0b1110)
			i += 3;
		else if (str[i] >> 3 == 0b11110)
			i += 4;
		else
			i++;
		res++;
	}
	return res;
}
