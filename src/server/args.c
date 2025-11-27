#include "server.h"
#include <limits.h>
#include <errno.h>

static void open_dll_room(uint8_t room_type, char *path, server_t *srv) {
	(void)room_type;
	(void)path;
	(void)srv;

}

static int read_rooms(json_value_t *json, server_t *srv) {
	json_value_t *lst = json_object_get(json, "rooms");
	if (!lst)
		return 0;
	if (lst->type != JSON_ARRAY) {
		PRINT_ERR("cheese: config: value 'rooms' must be an array\n");
		return 1;
	}
	for (int i = 0; i < lst->val.arr->len; i++) {
		json_value_t *val = json_array_get(lst, i);
		if (val->type != JSON_ARRAY || val->val.arr->len != 2
			|| json_array_get(val, 0)->type != JSON_NUMBER
			|| json_array_get(val, 1)->type != JSON_STRING
			|| json_array_get(val, 0)->val.num >= 256
			|| json_array_get(val, 0)->val.num < 0
		) {
			PRINT_ERR("cheese: config: element %d of rooms invalid skipping", i);
			continue;
		}
		int room_type = json_array_get(val, 0)->val.num;
		char *path = json_array_get(val, 1)->val.str;
		open_dll_room((uint8_t)room_type, path, srv);
	}
	return 0;
}

static int read_json(json_value_t *json, server_t *srv) {
	if (!JSON_CHECK_TYPE(json, JSON_OBJECT)) {
		PRINT_ERR("cheese: config: json must be an object\n");
		return 1;
	}
	json_value_t *val = json_object_get(json, "port");
	if (val && val->type != JSON_NUMBER) {
		PRINT_ERR("cheese: config: value 'port' must be an integer");
		return 1;
	}
	if ((int)val->val.num != val->val.num) {
		PRINT_ERR("cheese: config: value 'port' must be an integer");
		return 1;
	}
	if (val)
		srv->port = val->val.num;

	if (read_rooms(json, srv))
		return 1;
	
	val = json_object_get(json, "path");
	if (val && val->type != JSON_STRING) {
		PRINT_ERR("cheese: config: value 'path' must be a string");
		return 1;
	}
	if (val)
		chdir(val->val.str);
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
