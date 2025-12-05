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

	board->players[new_piece->color].nb_piece++;
	if (new_piece->type == KING)
		board->players[new_piece->color].nb_kings++;
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

			i++;
			if (cur_piece == ' ')
				continue ;
			prepare_tile(board, cur_piece, j, i - 1);
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
	board->default_moves = malloc((board->height + 1) * sizeof(char *));
	if (!board->default_moves)
		exit(1);
	for (int j = 0; j < board->height; j++) {
		board->default_moves[j] = calloc((board->width + 1), sizeof(char));
		if (!board->default_moves[j])
			exit(1);
	}
	for (int i = 0; i < board->nb_piece; i++) {
		char	**map = NULL;

		map = malloc((board->height + 1) * sizeof(char *));
		if (!map)
			exit(1);

		for (int j = 0; j < board->height; j++) {
			map[j] = malloc((board->width + 1) * sizeof(char));
			if (!map[j])
				exit(1);
		}
		map[board->height] = NULL;
		board->pieces[i]->possible_moves = map;
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
	board->occupied_map = malloc((board->height + 1) * sizeof(char *));
	if (!board->occupied_map)
		exit(1);
	for (int i = 0; i < board->height; i++) {
		board->occupied_map[i] = malloc((board->width + 1) * sizeof(char));
		if (!board->occupied_map[i])
			exit(1);
	}
	board->nb_player = 2; //TODO change to nb of player
	board->players = malloc(sizeof(player_t) * (board->nb_player + 1));
	for (int i = 0; i < board->nb_player; i++) {
		board->players[i] = (player_t){0};
		board->players[i].color = i;
	}
	init_tiles(board_str, board);
	init_possible_boards(board);
	free(board_str);
	board->copy_board = clone_board(board);
}
