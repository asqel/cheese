#include "cheese.h"

piece_t	*create_new_piece(char id)
{
	int i = -1;
	while (g_config_file.piece_types[++i]) {
		piece_type_t *cur_type = g_config_file.piece_types[i];
		if (cur_type->id != id)
			continue ;
		piece_t *dest = calloc(1, sizeof(piece_t));
		if (!dest)
			exit(1);

		dest->type = cur_type;
		dest->hp = cur_type->max_hp;
		dest->attack_power = cur_type->attack_power;
		return (dest);
	}
	return (NULL);
}
