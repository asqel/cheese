#include "cheese.h"

void	print_board(board_t	*board)
{
	write(1, "\e[0;0m\e[?25l", 12);
	printf("┌──");
	for (int i = 0; i < board->width - 1; i++)
		printf("─┬──");
	printf("─┐\n");

	for (int i = 0; i < board->height - 1; i++) {
		for (int j = 0; j < board->width; j++)
			printf("│   ");
		printf("│\n├──");
		for (int j = 0; j < board->width - 1; j++)
			printf("─┼──");
		printf("─┤\n");
	}
	for (int i = 0; i < board->width; i++)
		printf("│   ");
	printf("│\n└──");
	for (int i = 0; i < board->width - 1; i++)
		printf("─┴──");
	printf("─┘\n");
	for (int i = 0; i < (board->height * 2); i++)
		printf("%s", CURSOR_UP);
	for (int j = 0; j < board->height; j++) {
		printf("\r");
		for (int i = 0; i < (j * 2); i++)
			printf("%s", CURSOR_DOWN);
		for (int i = 0; i < board->width; i++) {
			board->occupied_map[j][i] = board->tiles[j][i].nb_piece != 0;
			if (!board->tiles[j][i].nb_piece)
				continue ;
			printf("%s", "\r");
			printf("%s", CURSOR_RIGHT);
			printf("%s", CURSOR_RIGHT);
			for (int k = 0; k < (i * 4); k++)
				printf("%s", CURSOR_RIGHT);
			printf("%s", board->tiles[j][i].pieces[0].character);
		}
		for (int i = 0; i < (j * 2); i++)
			printf("%s", CURSOR_UP);
	}
	printf("\r");
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
			case 10:
				if (!confirm && board->occupied_map[y][x]) {
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
