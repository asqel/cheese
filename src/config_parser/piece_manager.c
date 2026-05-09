#include <ctype.h>
#include "../../include/cheese.h"

void *retrieve_function(char *func_name);

static int safe_atoi(char *str, int limit, int *error)
{
	int nb = 0;
	int sign = 1;

	if (str[0] == '-') {
		sign = -1;
		++str;
	}
	for (size_t i = 0; i < strlen(str); i++) {
		if (!isdigit(str[i])) {
			*error = 1;
			break ;
		}
		nb = (nb * 10) + (str[i] - '0');
	}
	nb = nb * sign;
	if (limit && nb > limit)
		*error = 1;
	return (nb);
}

static int handle_attributes_adding(piece_type_t *piece, char *key, char *value)
{
	int res = 0;

	if (!strcmp(key, "id") && strlen(value) == 1)
		piece->id = *value;
	else if (!strcmp(key, "char"))
		strncpy(piece->character, value, 5);
	else if (!strcmp(key, "hp"))
		piece->max_hp = safe_atoi(value, 0, &res);
	else if (!strcmp(key, "attack"))
		piece->attack_power = safe_atoi(value, 0, &res);
	else if (!strcmp(key, "nb_move"))
		piece->nb_move = safe_atoi(value, 0, &res);
	else if (!strcmp(key, "color"))
		piece->color = safe_atoi(value, 8, &res);
	else if (!strcmp(key, "default_move"))
		piece->default_move_func = retrieve_function(value);
	else if (!strcmp(key, "is_king"))
		piece->is_king = safe_atoi(value, 2, &res);
	else if (!strcmp(key, "is_cannibal"))
		piece->is_cannibal = safe_atoi(value, 2, &res);
	else {
		fprintf(stderr, "Warning: unrecognized token `%s'\n", key);
		res = 1;
	}
	return (1);
}

piece_type_t *update_current_piece(char *line, size_t line_size)
{
	static piece_type_t *cur_piece = NULL;
	static uint16_t cur_piece_id = 0;
	char *key, *value;

	if (!line) {
		cur_piece->piece_type_id = cur_piece_id++;
		piece_type_t *dest = cur_piece;
		cur_piece = NULL;
		printf("Returned piece |%s|\n", dest->character);
		return (dest);
	}
	if (!cur_piece) {
		cur_piece = malloc(sizeof(piece_type_t));
		if (!cur_piece)
			exit(1);
		cur_piece->is_king = 0; //TODO defaults
		cur_piece->is_cannibal = 0;
		cur_piece->attack_power = 1;
		cur_piece->nb_move = 1;
		cur_piece->max_hp = 2;
	}

	key = line;
	for (size_t i = 0; i < line_size; i++) {
		if ((key == line) && !isspace(line[i]))
			key = line + i;
		if (line[i] != ':')
			continue ;

		line[i] = 0;
		while (isspace(line[++i]))
			;
		value = line + i;
		break ;
	}
	handle_attributes_adding(cur_piece, key, value);
	return (NULL);
}
