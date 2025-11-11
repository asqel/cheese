#include "cheese.h"
#include "server.h"

#include <signal.h>
void lexit(void) {
	printf("\e[?1049l");
	fflush(stdout);
}

void on_sigint(int _) {
	(void)_;
	exit(1);
}

void	remove_piece(tile_t *target, int id)
{
	piece_t	piece = target->pieces[id];
	for (int i = (id + 1); i < target->nb_piece; i++)
		target->pieces[i - 1] = target->pieces[i];
	if (!--target->nb_piece) {
		free(target->pieces);
		target->pieces = NULL;
	}
	free(piece.character);
}

void	move_piece(board_t *board, int y, int x)
{
	board->selector.target_y = y;
	board->selector.target_x = x;
	tile_t	*origin_tile = &board->tiles[board->selector.origin_y][board->selector.origin_x];
	tile_t	*target_tile = &board->tiles[board->selector.target_y][board->selector.target_x];
	piece_t selected_piece = origin_tile->pieces[board->selector.origin_id];

	for (int i = (board->selector.origin_id + 1); i < origin_tile->nb_piece; i++)
		origin_tile->pieces[i - 1] = origin_tile->pieces[i];
	if (!--origin_tile->nb_piece) {
		free(origin_tile->pieces);
		origin_tile->pieces = NULL;
	}
	if (target_tile->nb_piece) {
		remove_piece(target_tile, 0);
		selected_piece.kill_count++;
	}
	selected_piece.move_counter++;
	target_tile->pieces = realloc(target_tile->pieces,
			sizeof(piece_t) * (target_tile->nb_piece + 1));
	if (!target_tile->pieces)
		exit(1);
	target_tile->pieces[target_tile->nb_piece++] = selected_piece;
}

void	highlight_board(board_t *board, int y, int x)
{
	int cursor_y;
	int	cursor_x;

	get_cursor_position(&cursor_x, &cursor_y);
	write(1, "\e[?25l", 6);
	for (int j = y; j > 0; j--)
		for (int k = 0; k < 2; k++)
			printf("%s", CURSOR_UP);
	for (int i = x; i > 0; i--)
		for (int k = 0; k < 4; k++)
			printf("%s", CURSOR_LEFT);
	for (int j = 0; j < board->height; j++) {
		printf("\r%s%s", CURSOR_RIGHT, CURSOR_RIGHT);
		for (int i = 0; i < board->width; i++) {
			printf("%s%s%s", board->possible_moves[j][i] ? BLUE_BG : BLACK_BG,
					get_tile_pieces(board, i, j), BLACK_BG);
			for (int k = 0; k < 3; k++)
				printf("%s", CURSOR_RIGHT);
		}
		for (int i = 0; i < 2; i++)
			printf("%s", CURSOR_DOWN);
	}
	printf("\033[%d;%dH\e[?25h", cursor_y, cursor_x);
	fflush(stdout);
}

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

void	free_board(board_t *board)
{
	for (int j = 0; j < board->height; j++) {
		free(board->occupied_map[j]);
		free(board->possible_moves[j]);
		for (int i = 0; i < board->width; i++) {
			for (int k = 0; k < board->tiles[j][i].nb_piece; k++)
				free(board->tiles[j][i].pieces[k].character);
			if (board->tiles[j][i].nb_piece)
				free(board->tiles[j][i].pieces);
		}
		free(board->tiles[j]);
	}
	free(board->occupied_map);
	free(board->possible_moves);
	free(board->tiles);
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

int	main(void) {
	board_t		board = {0};

	printf("\e[?1049h");
	fflush(stdout);
	atexit(lexit);
	signal(SIGINT, on_sigint);
	//srv_start(0, NULL);
	init_board("base", &board);
	while (1) {
		write(1, "\033[2J\033[H", 7);
		if (play(&board))
			break ;
	}
	free_board(&board);
	
	//getc(stdin);
	return (0);
}
