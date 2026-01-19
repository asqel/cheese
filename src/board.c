#include "cheese.h"

int	board_move_cursor(board_t *board, int *y, int *x) {
	while (1) {
		char key = read_char();
		if (key >= 'A' && key <= 'Z')
			key += 32;
		switch (key) {
			case 3:
				exit(1);
			case 'w':
				if (*y != 0) {
					printf("%s%s", CURSOR_UP, CURSOR_UP);
					(*y)--;
				}
				break ;
			case 's':
				if (*y != (board->height - 1)) {
					printf("%s%s", CURSOR_DOWN, CURSOR_DOWN);
					(*y)++;
				}
				break ;
			case 'a':
				if (*x != 0) {
					printf("%s%s%s%s", CURSOR_LEFT, CURSOR_LEFT, CURSOR_LEFT, CURSOR_LEFT);
					(*x)--;
				}
				break ;
			case 'd':
				if (*x != (board->width - 1)) {
					printf("%s%s%s%s", CURSOR_RIGHT, CURSOR_RIGHT, CURSOR_RIGHT, CURSOR_RIGHT);
					(*x)++;
				}
				break ;
			case 'g':
				board->debug = !board->debug;
				update_possible_moves(board, -1, -1);
				break ;
			case 'q':
				return (0);
			case '\n':
				return (1);
			case 27:
				return (2);
		}
		fflush(stdout);
	}
}

void	print_board(board_t	*board) {
	tile_t	**tiles = board->tiles;
	printf("\033[2J\033[0;0m\033[?25l\033[%d;0H", PROMO_OFFSET);
	fflush(stdout);
	printf("%*s", PROMO_OFFSET, "");
	for (int i = 0; i < (board->width + 1); i++) {
		if (i == 0)
			printf("%s", !tiles[0][0].is_blocked ? "┌──" : "   " );
		else if (i == board->width)
			printf("%s", !tiles[0][i - 1].is_blocked ? "─┐" : "  " );
		else {
			printf("%s", (!tiles[0][i - 1].is_blocked) ? "─" : " ");
			if (tiles[0][i - 1].is_blocked + tiles[0][i].is_blocked == 1)
				printf("%s", (tiles[0][i].is_blocked) ? "┐" : "┌");
			else
				printf("%s", (tiles[0][i].is_blocked) ? " " : "┬");
			printf("%s", (!tiles[0][i].is_blocked) ? "──" : "  ");
		}
	}
	for (int j = 0; j < board->height; j++) {
		printf("\n%*s", PROMO_OFFSET, "");
		for (int i = 0; i < (board->width + 1); i++) {
			if (i == 0)
				printf("%s", (!tiles[j][0].is_blocked) ? "│  " : "   ");
			else if (i == board->width)
				printf("%s", (!tiles[j][i - 1].is_blocked) ? " │" : "  ");
			else
				printf("%s", (!tiles[j][i - 1].is_blocked + !tiles[j][i].is_blocked) ? " │  " : "    ");
		}
		printf("\n%*s", PROMO_OFFSET, "");
		if (j == (board->height - 1))
			continue ;
		for (int i = 0; i < (board->width + 1); i++) {
			if (i == 0) {
				if ((tiles[j][0].is_blocked + tiles[j + 1][0].is_blocked) == 1)
					printf("%s", !tiles[j][0].is_blocked ? "└──" : "┌──");
				else
					printf("%s", tiles[j][0].is_blocked ? "   " : "├──");
			}
			else if (i == board->width) {
				if ((!tiles[j][i - 1].is_blocked + !tiles[j + 1][i - 1].is_blocked) == 1)
					printf("%s", tiles[j + 1][i - 1].is_blocked ? "─┘" : "─┐");
				else
					printf("%s", tiles[j][i - 1].is_blocked ? "  " : "─┤");
			}
			else {
				printf("%s", (!tiles[j + 1][i - 1].is_blocked + !tiles[j][i - 1].is_blocked) ? "─" : " ");
				int sum = !tiles[j][i].is_blocked + !tiles[j][i - 1].is_blocked +
						!tiles[j + 1][i].is_blocked + !tiles[j + 1][i - 1].is_blocked;
				if (sum == 0 || sum == 4)
					printf("%s", !tiles[j + 1][i].is_blocked ? "┼" : " ");
				else if (sum == 3)
					printf("┼");
				else if (sum == 2) {
					if (!(tiles[j][i - 1].is_blocked + tiles[j + 1][i].is_blocked) ||
						!(tiles[j][i].is_blocked + tiles[j + 1][i - 1].is_blocked))
						printf("┼");
					else if (!tiles[j + 1][i].is_blocked)
						printf("%s", (!tiles[j][i].is_blocked) ? "├" : "┬");
					else
						printf("%s", tiles[j][i].is_blocked ? "┤" : "┴");
				}
				else {
					if ((tiles[j + 1][i].is_blocked + tiles[j + 1][i - 1].is_blocked) == 1)
						printf("%s", tiles[j + 1][i].is_blocked ? "┐" : "┌");
					else
						printf("%s", tiles[j + 1][i].is_blocked ? "└" : "┘");
				}
				printf("%s", (!tiles[j + 1][i].is_blocked + !tiles[j][i].is_blocked) ? "──" : "  ");
			}
		}
	}
	int j = board->height - 1;
	for (int i = 0; i < (board->width + 1); i++) {
		if (i == 0)
			printf("%s", !tiles[j][0].is_blocked ? "└──" : "   " );
		else if (i == board->width)
			printf("%s", !tiles[j][i - 1].is_blocked ? "─┘" : "  " );
		else {
			printf("%s", (!tiles[j][i - 1].is_blocked) ? "─" : " ");
			if (tiles[j][i - 1].is_blocked + tiles[j][i].is_blocked == 1)
				printf("%s", (tiles[j][i].is_blocked) ? "┘" : "└");
			else
				printf("%s", (tiles[j][i].is_blocked) ? " " : "┴");
			printf("%s", (!tiles[j][i].is_blocked) ? "──" : "  ");
		}
	}
	printf("\n%*s", PROMO_OFFSET, "");
	for (int i = 0; i < board->height; i++)
		printf("%s%s", CURSOR_UP, CURSOR_UP);
	for (int j = 0; j < board->height; j++) {
		printf("\r%*s", PROMO_OFFSET, "");
		for (int i = 0; i < j; i++)
			printf("%s%s", CURSOR_DOWN, CURSOR_DOWN);
		for (int i = 0; i < board->width; i++) {
			tile_t	*tile = &board->tiles[j][i];
			board->occupied_map[j][i] = tile->nb_piece != 0;
			if (!tile->nb_piece)
				continue ;
			printf("\r%*s", PROMO_OFFSET, "");
			printf("%s", CURSOR_RIGHT);
			printf("%s", CURSOR_RIGHT);
			for (int k = 0; k < i; k++)
				printf("%s%s%s%s", CURSOR_RIGHT, CURSOR_RIGHT, CURSOR_RIGHT, CURSOR_RIGHT);
			piece_t	*piece = tile->pieces[0];
			if (piece->color == BOARD)
				continue ;
			for (int k = 0; k < tile->nb_piece; k++) {
				piece_t	*temp_piece = tile->pieces[k];
				if (temp_piece->type == KING && temp_piece->is_targeted == 1) {
					printf("%s", RED_BG);
					break ;
				}
			}
			printf("%s%s", piece->character, BLACK_BG);
		}
		for (int i = 0; i < j; i++)
			printf("%s%s", CURSOR_UP, CURSOR_UP);
	}
	printf("\r%*s", PROMO_OFFSET, "");
	for (int i = 0; i < (board->height + 1); i++)
		printf("%s%s", CURSOR_DOWN, CURSOR_DOWN);
	printf("\e[?25h");
	fflush(stdout);
}

void	print_end_message(board_t *board, char *msg) {
		print_board(board);
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
}

int	play(board_t *board)
{
	static int	x = 0;
	static int	y = 0;
	int	confirm = 0;

	for (int j = 0; j < board->height; j++)
		for (int i = 0; i < board->width; i++)
			board->tiles[j][i].is_targeted = 0;
	for (int i = 1; i < (board->nb_player + 1); i++) {
		if (board->players[i].nb_kings)
			continue ;
		char *msg;
		if (i == 1) //TODO change this horror
			msg = "White wins!";
		else if (i == 2)
			msg = "Black wins!";
		else
			msg = "Draw!";
		print_end_message(board, msg);
		return (1);
	}
	if (board->special_tile) {
		print_board(board);
		tile_t	*promo_tile = (tile_t *)board->special_tile;
		piece_t *ref = promo_tile->pieces[promo_tile->nb_piece - 1];
		int new_piece_type = promo_menu(!ref->color * (board->height + 3), ref->color, board) + 1;

		ref->type = new_piece_type;
		strcpy(ref->character, "♔");
		ref->character[2] += new_piece_type + ((ref->color - 1) * 6);
		board->special_tile = NULL;
	}
	update_possible_moves(board, -1, -1);
	for (int i = 1; i < (board->nb_player + 1); i++) {
		board->players[i].king_in_check = king_in_check(board, board->players[i].color);
		int	color = board->players[i].color;
		int	can_move = 0;
		for (int p = 0; p < board->nb_piece; p++) {
			piece_t	*cur = board->pieces[p];
			if (cur->is_dead || cur->color != color || !cur->can_move)
				continue ;
			can_move = 1;
			break ;
		}
		if (!can_move && !board->players[i].king_in_check) {
			print_end_message(board, "Stalemate");
			return (1);
		}
		else if (!can_move || board->players[i].king_in_check) {
			board->players[i].king_in_check += !can_move;
			update_possible_moves(board, -1, color);
			if (!can_move)
				while (1)
					;
		}
	}
	print_board(board);
	for (int i = 0; i < (x * 4); i++)
		printf("%s", CURSOR_RIGHT);
	for (int i = 0; i < ((board->height - y + 1) * 2); i++)
		printf("%s", CURSOR_UP);
	printf("%s%s", CURSOR_RIGHT, CURSOR_RIGHT);
	fflush(stdout);
	while (1) {
		int state = board_move_cursor(board, &y, &x);
		switch (state) {
			case 0:
				return (1);
			case 1:
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
				else if (confirm && (board->debug || board->possible_locations[y][x])) {
					//TODO make sure the nb of piece doesn't go over MAX_PIECE
					for (int i = 0; i < board->selected_piece->nb_move; i++) {
						board->selector.target_id = 0;
						if (board->tiles[y][x].nb_piece > 1)
							board->selector.target_id =
								choose_target_piece(board, board->selected_piece,
									&board->tiles[y][x]);
						move_piece(board, y, x);
						if (i != (board->selected_piece->nb_move - 1)) {
							board->selected_piece->can_move = simulate_piece(board, board->selected_piece);
							if (!board->selected_piece->can_move)
								break ;
							highlight_board(board, y, x);
							while (1) {
								while (board_move_cursor(board, &y, &x) != 1)
									;
								if (board->selected_piece->possible_locations[y][x])
									break ;
							}
							board->selector.origin_id = board->selected_piece->tile_id;
						}
					}
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
			case 2:
				reset_possible_moves(board);
				highlight_board(board, y, x);
				confirm = 0;
				break ;
		}
		fflush(stdout);
		if (confirm == 2)
			return (0);
	}
}
