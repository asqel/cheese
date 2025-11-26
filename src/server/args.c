#include "server.h"
#include <limits.h>
#include <errno.h>

int read_json(json_value_t *json, server_t *srv) {
	(void)json;
	(void)srv;
	return 0;
}

int srv_parse_args(int argc, char **argv, server_t *srv) {
	if (argc > 1) {
		PRINT_ERR("cheese: server accept at most 1 argument\n");
		return 1;
	}
	if (argc == 0)
		return 0;
	json_value_t json;
	if (json_from_file(argv[0], &json)) { 
		PRINT_ERR("cheese: %s: error while reading json\n", argv[0]);
		return 1;
	}
	if (read_json(&json, srv)) {
		json_destroy(&json);
		return 1;
	}
	json_destroy(&json);
	return 0;
}
