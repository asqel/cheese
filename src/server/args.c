#include "server.h"
#include <limits.h>
#include <errno.h>

static void open_dll_room(uint8_t room_type, char *path) {
	if (srv->room_libs[room_type].handler) {
		PRINT_ERR("room type %d redefined, skipping...\n", room_type);
		return ;
	}
	room_lib_t *room = &srv->room_libs[room_type];
	room->handler = dlopen(path, RTLD_NOW);
	if (!room->handler) {
		PRINT_ERR("%s, skipping...\n", dlerror());
		return ;
	}
	room->init = dlsym(room->handler, "init_func");
	room->free = dlsym(room->handler, "free_func");
	room->join = dlsym(room->handler, "join_func");
	room->leave = dlsym(room->handler, "leave_func");
	room->move = dlsym(room->handler, "move_func");
	room->recv = dlsym(room->handler, "recv_func");
	room->reset = dlsym(room->handler, "reset_func");
	room->start = dlsym(room->handler, "start_func");
	if (!room->init)
		PRINT_ERR("%s: missing init_func\n", path);
	if (!room->free)
		PRINT_ERR("%s: missing free_func\n", path);
	if (!room->join)
		PRINT_ERR("%s: missing join_func\n", path);
	if (!room->leave)
		PRINT_ERR("%s: missing leave_func\n", path);
	if (!room->move)
		PRINT_ERR("%s: missing move_func\n", path);
	if (!room->recv)
		PRINT_ERR("%s: missing recv_func\n", path);
	if (!room->reset)
		PRINT_ERR("%s: missing reset_func\n", path);
	if (!room->start)
		PRINT_ERR("%s: missing start_func\n", path);
	if (room->init && room->free && room->join && room->leave
		&& room->move && room->recv && room->reset && room->start) {
		printf("adding %s as room type %d\n", path, room_type);
		return ;
	}
	dlclose(room->handler);
	room->handler = NULL;
	PRINT_ERR("\tskipping...\n");
}

static int read_rooms(json_value_t *json) {
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
			PRINT_ERR("cheese: config: element %d of rooms invalid, skipping...\n", i);
			continue;
		}
		int room_type = json_array_get(val, 0)->val.num;
		char *path = json_array_get(val, 1)->val.str;
		open_dll_room((uint8_t)room_type, path);
	}
	return 0;
}

static int read_json(json_value_t *json) {
	if (!JSON_CHECK_TYPE(json, JSON_OBJECT)) {
		PRINT_ERR("cheese: config: json must be an object\n");
		return 1;
	}
	json_value_t *val = json_object_get(json, "port");
	if (val && val->type != JSON_NUMBER) {
		PRINT_ERR("cheese: config: value 'port' must be an integer\n");
		return 1;
	}
	if ((int)val->val.num != val->val.num) {
		PRINT_ERR("cheese: config: value 'port' must be an integer\n");
		return 1;
	}
	if (val)
		srv->port = val->val.num;

	if (read_rooms(json))
		return 1;
	
	val = json_object_get(json, "path");
	if (val && val->type != JSON_STRING) {
		PRINT_ERR("cheese: config: value 'path' must be a string\n");
		return 1;
	}
	if (val)
		chdir(val->val.str);
	return 0;
}

int srv_parse_args(int argc, char **argv) {
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
	if (read_json(&json)) {
		json_destroy(&json);
		return 1;
	}
	json_destroy(&json);
	return 0;
}
