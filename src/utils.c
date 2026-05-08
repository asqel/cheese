#include "cheese.h"
#include "oeuf.h"

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

void	free_board(board_t *board)
{
	for (int j = 0; j < board->height; j++) {
		for (int i = 0; i < board->width; i++) {
			if (board->tiles[j][i].pieces)
				free(board->tiles[j][i].pieces);
		}
		free(board->tiles[j]);
	}
	oe_strarr_free(board->occupied_map, board->height);
	free(board->tiles);
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
