#include "cheese.h"

void	print_board(board_t	*board)
{
	printf("\033[2J\033[0;0m\033[?25l\033[%d;0H", PROMO_OFFSET);
	fflush(stdout);
	printf("%*s┌──", PROMO_OFFSET, "");
	for (int i = 0; i < board->width - 1; i++)
		printf("─┬──");
	printf("─┐\n%*s", PROMO_OFFSET, "");

	for (int i = 0; i < board->height - 1; i++) {
		for (int j = 0; j < board->width; j++)
			printf("│   ");
		printf("│\n%*s├──", PROMO_OFFSET, "");
		for (int j = 0; j < board->width - 1; j++)
			printf("─┼──");
		printf("─┤\n%*s", PROMO_OFFSET, "");
	}
	for (int i = 0; i < board->width; i++)
		printf("│   ");
	printf("│\n%*s└──", PROMO_OFFSET, "");
	for (int i = 0; i < board->width - 1; i++)
		printf("─┴──");
	printf("─┘\n%*s", PROMO_OFFSET, "");
	for (int i = 0; i < (board->height * 2); i++)
		printf("%s", CURSOR_UP);
	for (int j = 0; j < board->height; j++) {
		printf("\r%*s", PROMO_OFFSET, "");
		for (int i = 0; i < (j * 2); i++)
			printf("%s", CURSOR_DOWN);
		for (int i = 0; i < board->width; i++) {
			board->occupied_map[j][i] = board->tiles[j][i].nb_piece != 0;
			if (!board->tiles[j][i].nb_piece)
				continue ;
			printf("\r%*s", PROMO_OFFSET, "");
			printf("%s", CURSOR_RIGHT);
			printf("%s", CURSOR_RIGHT);
			for (int k = 0; k < (i * 4); k++)
				printf("%s", CURSOR_RIGHT);
			printf("%s", board->tiles[j][i].pieces[0]->character);
		}
		for (int i = 0; i < (j * 2); i++)
			printf("%s", CURSOR_UP);
	}
	printf("\r%*s", PROMO_OFFSET, "");
	for (int i = 0; i < ((board->height + 1) * 2); i++)
		printf("%s", CURSOR_DOWN);
	printf("\e[?25h");
	fflush(stdout);
}

int	play(board_t *board)
{
	static int	x = 0;
	static int	y = 0;
	int	confirm = 0;

	print_board(board);
	if (!board->white_kings || !board->black_kings) {
		char *msg;
		if (board->white_kings)
			msg = "White wins!";
		else if (board->black_kings)
			msg = "Black wins!";
		else
			msg = "Draw!";
		printf("\033[?25l");
		printf("%*s%s\n", (int)((board->width * 4 + 1) - strlen(msg)) / 2, "", msg);
		printf("%*s", PROMO_OFFSET, "");
		msg = "Press Enter to continue";
		printf("%*s%s", (int)((board->width * 4 + 1) - strlen(msg)) / 2, "", msg);
		fflush(stdout);
		while (1)
			if (read_char() == 10)
				break ;
		printf("\033[?25h");
		return (1);
	}
	if (board->promo_tile) {
		piece_t *ref = board->promo_tile->pieces[board->promo_tile->nb_piece - 1];
		int new_piece_type = promo_menu(!ref->color * (board->height + 3), ref->color, board) + 1;

		ref->type = new_piece_type;
		strcpy(ref->character, "♔");
		ref->character[2] += new_piece_type + (ref->color * 6);
		board->promo_tile = NULL;
		print_board(board);
	}
	for (int i = 0; i < (x * 4); i++)
		printf("%s", CURSOR_RIGHT);
	for (int i = 0; i < ((board->height - y + 1) * 2); i++)
		printf("%s", CURSOR_UP);
	printf("%s%s", CURSOR_RIGHT, CURSOR_RIGHT);
	fflush(stdout);
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
				if (y != (board->height - 1)) {
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
				if (x != (board->width - 1)) {
					for (int i = 0; i < 4; i++)
						printf("%s", CURSOR_RIGHT);
					x++;
				}
				break ;
			case 'g':
				board->debug = !board->debug;
				break ;
			case 10:
				if (!confirm && board->occupied_map[y][x]) {
					board->selector.origin_id = 0;
					if (board->tiles[y][x].nb_piece > 1)
						board->selector.origin_id =
							choose_tile_piece_menu(board, &board->tiles[y][x], -1);
					reset_possible_moves(board);
					if (update_possible_moves(board, y, x)) {
						highlight_board(board, y, x);
						confirm = 1;
					}
				}
				else if (confirm && board->possible_moves[y][x]) {
					move_piece(board, y, x);
					confirm = 2;
				}
				else if (confirm && board->occupied_map[y][x]) {
					board->selector.origin_id = 0;
					if (board->tiles[y][x].nb_piece > 1)
						board->selector.origin_id =
							choose_tile_piece_menu(board, &board->tiles[y][x], -1);
					reset_possible_moves(board);
					update_possible_moves(board, y, x);
					highlight_board(board, y, x);
				}
				break ;
			case 27:
				reset_possible_moves(board);
				highlight_board(board, y, x);
				confirm = 0;
				break ;
		}
		fflush(stdout);
		if (confirm == 2)
			return (0);
		if (key == 'q')
			return (1);
	}
}
