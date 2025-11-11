#include "cheese.h"

piece_t	create_piece(char piece)
{
	piece_t	dest = {0};

	dest.color = WHITE;
	if (piece >= 'a') {
		dest.color = BLACK;
		piece -= 32;
	}
	switch (piece) {
		case 'P':
			dest.type = PAWN;
			strcpy(dest.character, "♙");
			break ;
		case 'R':
			dest.type = ROOK;
			strcpy(dest.character, "♖");
			break ;
		case 'N':
			dest.type = KNIGHT;
			strcpy(dest.character, "♘");
			break ;
		case 'B':
			dest.type = BISHOP;
			strcpy(dest.character, "♗");
			break ;
		case 'Q':
			dest.type = QUEEN;
			strcpy(dest.character, "♕");
			break ;
		case 'K':
			dest.type = KING;
			strcpy(dest.character, "♔");
			break ;
	};
	if (dest.color == BLACK)
		dest.character[2] += "♚"[2] - "♔"[2];
	return (dest);
}

void	init_tiles(char *board_str, board_t *board)
{
	board->tiles = malloc((board->height + 1) * sizeof(tile_t *));
	if (!board->tiles)
		exit(1);
	for (int j = 0; j < board->height; j++) {
		board->tiles[j] = calloc((board->width + 1), sizeof(tile_t));
		if (!board->tiles[j])
			exit(1);

		int i = 0;
		while (*board_str && *board_str != '\n') {
			tile_t	*cur_tile = &board->tiles[j][i++];
			char	cur_piece = *board_str++;

			if (cur_piece == ' ')
				continue ;
			if (cur_piece >= 'a')
				board->black_pieces++;
			else
				board->white_pieces++;
			cur_tile->pieces = realloc(cur_tile->pieces,
					(cur_tile->nb_piece + 1) * sizeof(piece_t));
			cur_tile->pieces[cur_tile->nb_piece++] = create_piece(cur_piece);
		}
		board_str++;
		while (i < board->width) {
			board->tiles[j][i].nb_piece = 0;
			board->tiles[j][i].pieces = NULL;
			i++;
		}
	}
	board->tiles[board->height] = NULL;
}

void	init_board(char *filepath, board_t *board)
{
	FILE	*f = fopen(filepath, "r");
	char	*board_str = NULL;
	char	line[256];

	if (!f) {
		fprintf(stderr, "Cannot open file %s\n", filepath);
		exit(1);
	}

	while (fgets(line, sizeof(line), f)) {
		size_t line_len = strlen(line);

		board_str = strjoin(board_str, line, 1);
		board->height++;
		board->width = max(board->width, line_len - 1);
	}
	fclose(f);
	board->occupied_map = calloc(board->height + 1, sizeof(char *));
	board->possible_moves = calloc(board->height + 1, sizeof(char *));
	if (!board->occupied_map || !board->possible_moves)
		exit(1);
	for (int i = 0; i < board->height; i++) {
		board->occupied_map[i] = calloc(board->width + 1, sizeof(char));
		board->possible_moves[i] = calloc(board->width + 1, sizeof(char));
		if (!board->occupied_map[i] || !board->possible_moves[i])
			exit(1);
	}
	init_tiles(board_str, board);
	free(board_str);
	board->copy_board = clone_board(board);
}
