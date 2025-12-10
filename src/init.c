#include "cheese.h"

void	prepare_tile(board_t *board, char piece, int j, int i) {
	tile_t	*cur_tile = &board->tiles[j][i];
	board->pieces = realloc(board->pieces, sizeof(piece_t *) * (board->nb_piece + 2));
	if (!board->pieces)
		print_error("Malloc error for board->pieces", 1);

	cur_tile->pieces = realloc(cur_tile->pieces,
		(cur_tile->nb_piece + 1) * sizeof(piece_t *));
	if (!cur_tile->pieces)
		print_error("Malloc error for tile->pieces", 1);

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
		print_error("Malloc error for board->logs", 1);
	
	board->tiles = malloc((board->height + 1) * sizeof(tile_t *));
	if (!board->tiles)
		print_error("Malloc error for board->tiles", 1);

	board->nb_piece = 0;
	for (int j = 0; j < board->height; j++) {
		board->tiles[j] = malloc((board->width + 1) * sizeof(tile_t));
		for (int i = 0; i < board->width; i++) {
			tile_t	*tile = &board->tiles[j][i];
			tile->y = j;
			tile->x = i;
			tile->color = (i + j) % 2;
			tile->nb_piece = 0;
			tile->tile_type = REAL_TILE;
			tile->pieces = NULL;
		}
		if (!board->tiles[j])
			print_error("Malloc error for board->tiles[j]", 1);

		int i = 0;
		while (*board_str && *board_str != '\n') {
			char	cur_piece = *board_str++;

			i++;
			if (cur_piece == ' ')
				continue ;
			prepare_tile(board, cur_piece, j, i - 1);
		}
		board_str++;
	}
	board->tiles[board->height] = NULL;
}

void	init_possible_boards(board_t *board)
{
	board->default_moves = malloc((board->height + 1) * sizeof(char **));
	if (!board->default_moves)
		print_error("Malloc error for board->default_moves", 1);
	for (int j = 0; j < board->height; j++) {
		board->default_moves[j] = calloc((board->width + 1), sizeof(char *));
		if (!board->default_moves[j])
			print_error("Malloc error for board->default_moves[j]", 1);
		for (int i = 0; i < board->width; i++) {
			board->default_moves[j][i] = malloc(board->nb_piece * sizeof(char));
			if (!board->default_moves[j][i])
				print_error("Malloc error for board->default_moves[j][i]", 1);
			board->default_moves[j][board->width] = NULL;
		}
	}
	board->default_moves[board->height] = NULL;
	for (int p = 0; p < board->nb_piece; p++) {
		char	***possible_map = NULL;
		char	**location_map = NULL;

		possible_map = malloc((board->height + 1) * sizeof(char **));
		location_map = malloc((board->height + 1) * sizeof(char *));
		if (!possible_map || !location_map)
			print_error("Malloc error for possible/location_map", 1);

		for (int j = 0; j < board->height; j++) {
			possible_map[j] = malloc((board->width + 1) * sizeof(char *));
			location_map[j] = malloc((board->width + 1) * sizeof(char *));
			if (!possible_map[j] || !location_map[j])
				print_error("Malloc error for possible/location_map[j]", 1);
			for (int i = 0; i < board->width; i++) {
				possible_map[j][i] = malloc(board->nb_piece * sizeof(char));
				if (!possible_map[j][i])
					print_error("Malloc error for possible_map[j][i]", 1);
				possible_map[j][board->width] = NULL;
			}
		}
		possible_map[board->height] = NULL;
		location_map[board->height] = NULL;
		board->pieces[p]->possible_moves = possible_map;
		board->pieces[p]->possible_locations = location_map;
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
		print_error("Malloc error for board->occupied_map", 1);
	for (int i = 0; i < board->height; i++) {
		board->occupied_map[i] = malloc((board->width + 1) * sizeof(char));
		if (!board->occupied_map[i])
			print_error("Malloc error for board->occupied_map[i]", 1);
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
