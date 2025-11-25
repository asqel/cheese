#include "server.h"

int srv_load_config(char *path) {
	FILE *f = fopen(path, "r");
	if (!f) {
		perror(path);
		return 1;
	}

	while (1) {
		char *line = NULL;
		ssize_t line_len = get_line(&line, NULL, f);
		if (line_len <= 0) {
			free(line);
			break;
		}
		line_len = strlen(line);
		if (line[0] == '#') {
			free(line);
			continue;
		}

	}
	fclose(f);
	return 0;
}
