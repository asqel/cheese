#include "cheese.h"
#include "server.h"

void	print_board(board_t	*board)
{
	write(1, "\e[0;0m", 7);
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
	printf("%s", "\r");
	for (int i = 0; i < ((board->height + 1) * 2); i++)
		printf("%s", CURSOR_DOWN);
	fflush(stdout);
}

void	free_board(board_t *board)
{
	for (int j = 0; j < board->height; j++) {
		for (int i = 0; i < board->width; i++) {
			for (int k = 0; k < board->tiles[j][i].nb_piece; k++)
				free(board->tiles[j][i].pieces[k].character);
			if (board->tiles[j][i].nb_piece)
				free(board->tiles[j][i].pieces);
		}
		free(board->tiles[j]);
	}
	free(board->tiles);
}

#include <signal.h>
void lexit(void) {
	printf("\e[?1049l");
	fflush(stdout);
}

void on_sigint(int) {
	exit(1);
}

int	main(void) {
	board_t		board = {0};

	printf("\e[?1049h");
	fflush(stdout);
	atexit(lexit);
	signal(SIGINT, on_sigint);
	srv_start(0, NULL);
	init_board("base", &board);
	print_board(&board);
	free_board(&board);
	
	getc(stdin);
}
