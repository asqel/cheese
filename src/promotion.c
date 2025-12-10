#include "cheese.h"

int	choose_piece(int limit_x, int limit_y)
{
	int		x = 0;
	int		y = 0;

	while (1) {
		char	key = read_char();
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

int	choose_target_piece(board_t *board, piece_t *src, tile_t *target_tile) {
	int	nb_pieces = 0;
	int board_cursor_y, board_cursor_x;
	int	menu_x, menu_y;

	char	*moves = src->possible_moves[target_tile->y][target_tile->x];
	for (int p = 0; p < target_tile->nb_piece; p++)
		if (moves[p]) 
			nb_pieces++;

	menu_x = (board->width + 3) * 4;
	menu_y = PROMO_OFFSET + ((board->height - nb_pieces) / 2) * 2;
	if ((board->height - nb_pieces) % 2)
		menu_y++;
	get_cursor_position(&board_cursor_x, &board_cursor_y);
	printf("\033[?25h\033[?25h");
	fflush(stdout);
	printf("\033[%d;%dH┌───┐", menu_y++, menu_x);
	int k = 0;
	for (int p = 0; p < target_tile->nb_piece; p++) {
		if (!moves[p])
			continue ;
		printf("\033[%d;%dH│ %s │", menu_y++, menu_x, target_tile->pieces[p]->character);
		if (k++ != (nb_pieces - 1))
			printf("\033[%d;%dH├───┤", menu_y++, menu_x);
	}
	printf("\033[%d;%dH└───┘", menu_y, menu_x);
	menu_y = PROMO_OFFSET + ((board->height - nb_pieces) / 2) * 2 + 1;
	if ((board->height - nb_pieces) % 2)
		menu_y++;
	printf("\033[%d;%dH", menu_y, menu_x + 2);
	fflush(stdout);
	int	requested_piece_base = choose_piece(1, nb_pieces);
	int	requested_piece = requested_piece_base;
	int	good_piece_saw = 0;
	for (int p = 0; p < target_tile->nb_piece; p++) {
		if (moves[p])
			good_piece_saw++;
		else
			requested_piece++;
		if (good_piece_saw > requested_piece_base)
			break ;
	}
	nb_pieces = target_tile->nb_piece - 1;
	if (requested_piece != nb_pieces) {
		piece_t	*save = target_tile->pieces[requested_piece];
		target_tile->pieces[requested_piece] = target_tile->pieces[nb_pieces];
		target_tile->pieces[nb_pieces] = save;
	}
	printf("\033[%d;%dH\033[?25h", board_cursor_y, board_cursor_x);
	fflush(stdout);
	return (nb_pieces);
}

int	choose_tile_piece_menu(board_t *board, tile_t *tile, int color)
{
	int		nb_pieces = tile->nb_piece;
	int 	board_cursor_y, board_cursor_x;
	int		menu_x, menu_y;

	for (int i = 0; i < tile->nb_piece; i++)
		if (tile->pieces[i]->color != color && color != -1)
			nb_pieces--;

	menu_x = (board->width + 3) * 4;
	menu_y = PROMO_OFFSET + ((board->height - nb_pieces) / 2) * 2;
	if ((board->height - nb_pieces) % 2)
		menu_y++;
	get_cursor_position(&board_cursor_x, &board_cursor_y);
	printf("\033[?25h\033[?25h");
	fflush(stdout);
	printf("\033[%d;%dH┌───┐", menu_y++, menu_x);
	int k = 0;
	for (int i = 0; i < tile->nb_piece; i++) {
		if (!(tile->pieces[i]->color == color || color == -1))
			continue ;
		printf("\033[%d;%dH│ %s │", menu_y++, menu_x, tile->pieces[i]->character);
		if (k++ != (nb_pieces - 1))
			printf("\033[%d;%dH├───┤", menu_y++, menu_x);
	}
	printf("\033[%d;%dH└───┘", menu_y, menu_x);
	menu_y = PROMO_OFFSET + ((board->height - nb_pieces) / 2) * 2 + 1;
	if ((board->height - nb_pieces) % 2)
		menu_y++;
	printf("\033[%d;%dH", menu_y, menu_x + 2);
	fflush(stdout);
	int	requested_piece_base = choose_piece(1, nb_pieces);
	int	requested_piece = requested_piece_base;
	int	good_piece_saw = 0;
	for (int i = 0; i < tile->nb_piece; i++) {
		if (tile->pieces[i]->color == color || color == -1)
			good_piece_saw++;
		else
			requested_piece++;
		if (good_piece_saw > requested_piece_base)
			break ;
	}
	nb_pieces = tile->nb_piece - 1;
	if (requested_piece != nb_pieces) {
		piece_t	*save = tile->pieces[requested_piece];
		tile->pieces[requested_piece] = tile->pieces[nb_pieces];
		tile->pieces[nb_pieces] = save;
	}
	printf("\033[%d;%dH\033[?25h", board_cursor_y, board_cursor_x);
	fflush(stdout);
	return (nb_pieces);
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
	int	requested_piece = choose_piece(nb_pieces, 1);
	printf("\033[%d;%dH\033[?25h", board_cursor_y, board_cursor_x);
	fflush(stdout);
	return (requested_piece);
}
