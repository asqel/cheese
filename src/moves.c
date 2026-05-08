#include "cheese.h"

void	highlight_board(board_t *board, int y, int x)
{
	int cursor_y;
	int	cursor_x;

	get_cursor_position(&cursor_x, &cursor_y);
	board->possible_locations = board->selected_piece->possible_locations;
	write(1, "\e[?25l", 6);
	for (int j = y; j > 0; j--)
		for (int k = 0; k < 2; k++)
			printf("%s", CURSOR_UP);
	for (int i = x; i > 0; i--)
		for (int k = 0; k < 4; k++)
			printf("%s", CURSOR_LEFT);
	for (int j = 0; j < board->height; j++) {
		printf("\r%*s%s%s", PROMO_OFFSET, "", CURSOR_RIGHT, CURSOR_RIGHT);
		for (int i = 0; i < board->width; i++) {
			piece_t	*piece = get_tile_piece(board, j, i);
			if (piece && piece->type->color == BOARD)
				piece = NULL;
			if (board->possible_locations[j][i])
				printf("%s", BLUE_BG);
			else if (piece && piece->type == KING && piece->is_targeted)
				printf("%s", RED_BG);
			printf("%s%s", (piece != NULL) ? piece->type->character : " ", BLACK_BG);
			for (int k = 0; k < 3; k++)
				printf("%s", CURSOR_RIGHT);
		}
		for (int i = 0; i < 2; i++)
			printf("%s", CURSOR_DOWN);
	}
	printf("\033[%d;%dH\e[?25h", cursor_y, cursor_x);
	fflush(stdout);
}
