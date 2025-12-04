#include "cheese.h"

void	prepare_tile(board_t *board, char piece, int j, int i) {
	tile_t	*cur_tile = &board->tiles[j][i];
	board->pieces = realloc(board->pieces, sizeof(piece_t *) * (board->nb_piece + 2));
	if (!board->pieces)
		exit(1);

	cur_tile->pieces = realloc(cur_tile->pieces,
			(cur_tile->nb_piece + 1) * sizeof(piece_t *));
	if (!cur_tile->pieces)
		exit(1);

	piece_t	*new_piece = create_piece(piece, board->nb_piece);
	new_piece->y = j;
	new_piece->x = i;

	board->pieces[board->nb_piece] = new_piece;
	board->pieces[++board->nb_piece] = NULL;

	if (piece >= 'a' && new_piece->type == KING)
		board->black_kings++;
	else if (new_piece->type == KING)
		board->white_kings++;
	cur_tile->pieces[cur_tile->nb_piece++] = new_piece;
}

void	init_tiles(char *board_str, board_t *board)
{
	board->logs = calloc(1, sizeof(move_logs_t));
	if (!board->logs)
		exit(1);
	
	board->tiles = malloc((board->height + 1) * sizeof(tile_t *));
	if (!board->tiles)
		exit(1);

	board->nb_piece = 0;
	for (int j = 0; j < board->height; j++) {
		board->tiles[j] = calloc((board->width + 1), sizeof(tile_t));
		if (!board->tiles[j])
			exit(1);

		int i = 0;
		while (*board_str && *board_str != '\n') {
			char	cur_piece = *board_str++;

			if (cur_piece == ' ') {
				i++;
				continue ;
			}
			prepare_tile(board, cur_piece, j, i++);
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

void	init_possible_boards(board_t *board)
{
	for (int j = 0; j < board->nb_piece; j++) {
		char	**map = board->pieces[j]->possible_moves;

		map = malloc((board->height + 1) * sizeof(char *));
		if (!map)
			exit(1);

		for (int i = 0; i < board->width; i++) {
			map[i] = calloc(board->width + 1, sizeof(char));
			if (!map[i])
				exit(1);
		}
		map[board->height] = NULL;
	}
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
