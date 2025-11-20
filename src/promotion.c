#include "cheese.h"

int	choose_tile_piece(int limit_x, int limit_y)
{
	int		x = 0;
	int		y = 0;

	while (1) {
		char	key = read_char();
		if (key >= 'A' && key <= 'Z')
			key += 3;
		switch (key) {
			case 'w':
				if (y != 0) {
					for (int i = 0; i < 2; i++)
						printf("%s", CURSOR_UP);
					y--;
				}
				break ;
			case 's':
				if (y != (limit_y - 1)) {
					for (int i = 0; i < 2; i++)
						printf("%s", CURSOR_DOWN);
					y++;
				}
				break ;
			case 'a':
				if (x != 0) {
					for (int i = 0; i < 4; i++)
						printf("%s", CURSOR_LEFT);
					x--;
				}
				break ;
			case 'd':
				if (x != (limit_x - 1)) {
					for (int i = 0; i < 4; i++)
						printf("%s", CURSOR_RIGHT);
					x++;
				}
				break ;
			case 10:
				return (y);
		}
		fflush(stdout);
	}
}

int	choose_tile_piece_menu(board_t *board, tile_t *tile)
{
	int		nb_pieces = tile->nb_piece;
	int 	board_cursor_y, board_cursor_x;
	int		menu_x, menu_y;

	menu_x = (board->width + 3) * 4;
	menu_y = ((board->height - nb_pieces) / 2) * 4 - 1;
	get_cursor_position(&board_cursor_x, &board_cursor_y);
	printf("\033[?25h]\033[?25h");
	fflush(stdout);
	printf("\033[%d;%dH┌───┐", menu_y++, menu_x);
	for (int i = 0; i < nb_pieces; i++) {
		printf("\033[%d;%dH│ %s │", menu_y++, menu_x, tile->pieces[i].character);
		if (i != (nb_pieces - 1))
			printf("\033[%d;%dH├───┤", menu_y++, menu_x);
	}
	printf("\033[%d;%dH└───┘", menu_y, menu_x);
	menu_y = ((board->height - nb_pieces) / 2) * 4;
	printf("\033[%d;%dH", menu_y, menu_x + 2);
	fflush(stdout);
	int	requested_piece = choose_tile_piece(1, nb_pieces);
	printf("\033[%d;%dH\033[?25h", board_cursor_y, board_cursor_x);
	fflush(stdout);
	return (requested_piece);
}

int	choose_promo_piece(const int limit_x, const int limit_y)
{
	int		x = 0;
	int		y = 0;

	while (1) {
		char key = read_char();
		if (key >= 'A' && key <= 'Z')
			key += 32;
		switch (key) {
			case 'w':
				if (y != 0) {
					for (int i = 0; i < 2; i++)
						printf("%s", CURSOR_UP);
					y--;
				}
				break ;
			case 's':
				if (y != (limit_y - 1)) {
					for (int i = 0; i < 2; i++)
						printf("%s", CURSOR_DOWN);
					y++;
				}
				break ;
			case 'a':
				if (x != 0) {
					for (int i = 0; i < 4; i++)
						printf("%s", CURSOR_LEFT);
					x--;
				}
				break ;
			case 'd':
				if (x != (limit_x - 1)) {
					for (int i = 0; i < 4; i++)
						printf("%s", CURSOR_RIGHT);
					x++;
				}
				break ;
			case 10:
				return ((y * limit_x) + x);
		}
		fflush(stdout);
	}
}

int	promo_menu(int y, int color, board_t *board)
{
	int		nb_pieces = 4;
	int		nb_spaces = PROMO_OFFSET + board->width / nb_pieces * 4;
	char	cur_piece[5];
	int 	board_cursor_y, board_cursor_x;

 	strcpy(cur_piece, "♕");
	cur_piece[2] += (color * 6);
	get_cursor_position(&board_cursor_x, &board_cursor_y);
	printf("\033[%d;%dH\033[?25h", 1 + (y * 2), 0);
	write(1, "\e[?25l", 6);
	printf("%*s┌──", nb_spaces, "");
	for (int i = 0; i < 3; i++)
		printf("─┬──");
	printf("─┐\n%*s│", nb_spaces, "");

	for (int i = 0; i < 4; i++) {
		printf(" %s │", cur_piece);
		cur_piece[2]++;
	}
	printf("\n%*s└──", nb_spaces, "");
	for (int i = 0; i < 3; i++)
		printf("─┴──");
	printf("─┘%s\r", CURSOR_UP);
	for (int i = 0; i < nb_spaces + 2; i++)
		printf("%s", CURSOR_RIGHT);
	printf("\033[?25h");
	fflush(stdout);
	int	requested_piece = choose_promo_piece(nb_pieces, 1);
	printf("\033[%d;%dH\033[?25h", board_cursor_y, board_cursor_x);
	fflush(stdout);
	return (requested_piece);
}
