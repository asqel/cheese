#include <ctype.h>
#include "cheese.h"
#include "default_chess_conf.h"

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

static int handle_attributes_adding(piece_type_t *piece,
		char *key, char *value, int changing_default)
{
	int res = 0;

	if (!strcmp(key, DEFAULT_CHARACTER_STR))
		strncpy(piece->character, value, sizeof(piece->character));
	else if (!strcmp(key, DEFAULT_MAX_HP_STR))
		piece->max_hp = safe_atoi(value, 0, &res);
	else if (!strcmp(key, DEFAULT_ATTACK_POWER_STR))
		piece->attack_power = safe_atoi(value, 0, &res);
	else if (!strcmp(key, DEFAULT_NB_MOVE_STR))
		piece->nb_move = safe_atoi(value, 0, &res);
	else if (!strcmp(key, DEFAULT_COLOR_STR))
		piece->color = safe_atoi(value, 8, &res);
	else if (!strcmp(key, DEFAULT_IS_KING_STR))
		piece->is_king = safe_atoi(value, 2, &res);
	else if (!strcmp(key, DEFAULT_IS_CANNIBAL_STR))
		piece->is_cannibal = safe_atoi(value, 2, &res);
	else if (!strcmp(key, DEFAULT_MOVE_FUNC_STR))
		piece->default_move_func = retrieve_function(value);
	else if (!strcmp(key, DEFAULT_HURT_FUNC_STR))
		piece->default_hurt_func = retrieve_function(value);
	else if (!strcmp(key, DEFAULT_ATTACK_FUNC_STR))
		piece->default_attack_func = retrieve_function(value);
	else if (!strcmp(key, DEFAULT_DEATH_FUNC_STR))
		piece->default_death_func = retrieve_function(value);
	else if (!strcmp(key, DEFAULT_KILL_FUNC_STR))
		piece->default_kill_func = retrieve_function(value);
	else if (!strcmp(key, DEFAULT_ID_STR) && strlen(value) == 1) {
		if (changing_default) {
			fprintf(stderr, "Error: modifying 'id' is forbidden in default config\n");
			res = 1;
		}
		piece->id = *value;
	}
	else {
		fprintf(stderr, "Warning: unrecognized token `%s'\n", key);
		res = 1;
	}
	return (1);
}

void init_default_config(piece_type_t *piece)
{
	piece_type_t *conf = &g_config_file.default_conf;

	if (piece == NULL) { //init default config
		conf->max_hp = DEFAULT_MAX_HP;
		conf->color = DEFAULT_COLOR;
		conf->is_king = DEFAULT_IS_KING;
		conf->attack_power = DEFAULT_ATTACK_POWER;
		conf->nb_move = DEFAULT_NB_MOVE;
		conf->is_cannibal = DEFAULT_IS_CANNIBAL;
		strncpy(conf->character, DEFAULT_CHARACTER, sizeof(conf->character));

		conf->default_move_func = retrieve_function(DEFAULT_MOVE_FUNC);
		conf->default_hurt_func = retrieve_function(DEFAULT_HURT_FUNC);
		conf->default_attack_func = retrieve_function(DEFAULT_ATTACK_FUNC);
		conf->default_death_func = retrieve_function(DEFAULT_DEATH_FUNC);
		conf->default_kill_func = retrieve_function(DEFAULT_KILL_FUNC);
		return ;
	}
	piece->max_hp = conf->max_hp;
	piece->color = conf->color;
	piece->is_king = conf->is_king;
	piece->attack_power = conf->attack_power;
	piece->nb_move = conf->nb_move;
	piece->is_cannibal = conf->is_cannibal;
	strncpy(piece->character, conf->character, sizeof(conf->character));

	piece->default_move_func = conf->default_move_func;
	piece->default_hurt_func = conf->default_hurt_func;
	piece->default_attack_func = conf->default_attack_func;
	piece->default_death_func = conf->default_death_func;
	piece->default_kill_func = conf->default_kill_func;
}

piece_type_t *update_current_piece(char *line, size_t line_size, int is_default)
{
	static piece_type_t *cur_piece = NULL;
	static uint16_t cur_piece_id = 0;
	char *key, *value;

	if (!line) {
		if (is_default) {
			cur_piece = NULL;
			return (NULL);
		}
		cur_piece->piece_type_id = cur_piece_id++;
		piece_type_t *dest = cur_piece;
		cur_piece = NULL;
		return (dest);
	}

	if (is_default)
		cur_piece = &g_config_file.default_conf;
	else if (!cur_piece) {
		cur_piece = malloc(sizeof(piece_type_t));
		if (!cur_piece)
			exit(1);
		init_default_config(cur_piece);
		cur_piece->id = 0;
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
	handle_attributes_adding(cur_piece, key, value, is_default);
	return (NULL);
}
