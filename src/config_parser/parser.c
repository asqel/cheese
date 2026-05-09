#include "cheese.h"

static piece_type_t **piece_types = NULL;

void update_element(char current_read, char *line_ptr, size_t read)
{
	void *res = 0;

	switch (current_read) {
		case 'p':
			res = update_current_piece(line_ptr, read, 0);
			break ;
		case 'd':
			res = update_current_piece(line_ptr, read, 1);
			break ;
	}
	if (!res)
		return ;
	switch (current_read) {
		case 'p':
			piece_type_t *new_piece_type = (piece_type_t *)res;
			uint16_t piece_id = new_piece_type->piece_type_id;
			piece_types = realloc(piece_types, (piece_id + 2) * sizeof(piece_type_t));
			if (!piece_types)
				exit(1);
			piece_types[piece_id] = new_piece_type;
			piece_types[piece_id + 1] = NULL;
			break ;
	}
}

void parse_config_file(char *filepath)
{
	char *line_ptr = NULL;
	size_t buffer_limit = 0;
	ssize_t read;
	char current_read = 0;

	FILE *fp = fopen(filepath, "r");
	if (!fp)
		exit(1);

	init_default_config(NULL);
	while ((read = getline(&line_ptr, &buffer_limit, fp)) != -1) {
		if (read <= 1)
			continue ;

		if (!isspace(line_ptr[0])) {
			update_element(current_read, 0, 0);
			current_read = 0;
		}

		for (size_t i = read - 1; i != 0; --i) {
			if (!isspace(line_ptr[i]))
				break ;
			line_ptr[i] = 0;
		}
		update_element(current_read, line_ptr, read);
		if (!strcmp(line_ptr, "piece:"))
			current_read = 'p';
		else if (!strcmp(line_ptr, "default:"))
			current_read = 'd';
	}
	update_element(current_read, 0, 0);
	free(line_ptr);
	fclose(fp);
	g_config_file.piece_types = piece_types;
}
