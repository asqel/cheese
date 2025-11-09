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
			dest.character = strdup("♙");
			break ;
		case 'R':
			dest.type = ROOK;
			dest.character = strdup("♖");
			break ;
		case 'N':
			dest.type = KNIGHT;
			dest.character = strdup("♘");
			break ;
		case 'B':
			dest.type = BISHOP;
			dest.character = strdup("♗");
			break ;
		case 'Q':
			dest.type = QUEEN;
			dest.character = strdup("♕");
			break ;
		case 'K':
			dest.type = KING;
			dest.character = strdup("♔");
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
		board->tiles[board->width] = NULL;
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
	printf("%s", board_str);
	printf("Width = %d, Height = %d\n", board->width, board->height);
	init_tiles(board_str, board);
	free(board_str);
	printf("%d white pieces, %d black pieces\n", board->white_pieces, board->black_pieces);
}
