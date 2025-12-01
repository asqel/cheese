#include "server.h"

static int get_infos(void *data, uint16_t len, char **name, char **hash, client_t *clt, size_t additional) {
	size_t name_len = 0;
	while (name_len < len && ((char *)data)[name_len])
		name_len++;
	if (name_len == 0 || name_len > ROOM_NAME_LEN) {
		srv_send_err(clt, OPC_ERR_NAME_LEN);
		return 1;
	}
	if (len - (name_len + 1) - additional != 64) {
		srv_send_err(clt, OPC_ERR_INVALID_DATA);
		return 1;
	}
	if (!srv_is_name_valid(data)) {
		srv_send_err(clt, OPC_ERR_NAME_INVALID);
		return 1;	
	}
	*name = (char *)data;
	*hash = (char *)data + name_len + 1;
	return 0;
}

int srv_create_room(client_t *clt, void *data, uint16_t len, server_t *srv) {
	char *name = NULL;
	char *hash = NULL;
	if (get_infos(data, len, &name, &hash, clt, 1))
		return 0;
	if (clt->room_name[0]) {
		srv_send_err(clt, OPC_ERR_ALREADY_IN_ROOM);
		return 0;
	}
	if (oe_hashmap_get(&srv->rooms, name)) {
		srv_send_err(clt, OPC_ERR_NAME_TAKEN);
		return 0;
	}

	uint8_t room_type = *((uint8_t *)data + len - 1);
	if (!srv->room_libs[room_type].handler) {
		srv_send_err(clt, OPC_ERR_ROOM_TYPE);
		return 0;
	}
	room_info_t *room = malloc(sizeof(room_info_t));
	memset(room, 0, sizeof(room_info_t));
	strcpy(room->name, name);
	room->players = oe_strarr_append(room->players, strdup(clt->name), NULL, 0);
	memcpy(room->passwd_hash, hash, 64);
	strcpy(clt->room_name, name);
	room->type = room_type;
	
	room_lib_t *room_lib = &srv->room_libs[room_type];
	if (room_lib->init(room)) {
		free(room);
		free(room->players[0]);
		free(room->players);
		clt->room_name[0] = '\0';
		return 0;
	}
	oe_hashmap_set(&srv->rooms, name, room);
	srv_send_success(clt, OPC_CREATE_ROOM);
	return 0;
}

int srv_join_room(client_t *clt, void *data, uint16_t len, server_t *srv) {
	char *name = NULL;
	char *hash = NULL;
	if (get_infos(data, len, &name, &hash, clt, 0))
		return 0;
	if (clt->room_name[0]) {
		srv_send_err(clt, OPC_ERR_ALREADY_IN_ROOM);
		return 0;
	}	
	room_info_t *room = oe_hashmap_get(&srv->rooms, name);
	if (!room || strcmp(room->passwd_hash, hash)) {
		srv_send_err(clt, OPC_ERR_WRONG_PASSW);
	}
	if (srv_on_room_join(oe_hashmap_get(&srv->rooms, name), clt, srv))
		return 0;

	uint8_t data_success[sizeof(uint32_t) + 1];
	*(uint32_t *)data_success = OPC_JOIN;
	data_success[sizeof(uint32_t)] = room->type;
	srv_send(clt, OPC_SUCCESS, data_success, sizeof(uint32_t) + 1);
	return 0;
}

int srv_exit_room(client_t *clt, void *data, uint16_t len, server_t *srv) {
	(void)data;
	if (len) {
		srv_send_err(clt, OPC_ERR_INVALID_DATA);
		return 0;
	}
	if (!clt->room_name[0]) {
		srv_send_err(clt, OPC_ERR_NOT_IN_ROOM);
		return 0;
	}
	srv_on_room_exit(oe_hashmap_get(&srv->rooms, clt->room_name), clt, srv);
	srv_send_success(clt, OPC_EXIT_ROOM);
	return 0;
}

void srv_on_room_exit(room_info_t *room, client_t *clt, server_t *srv) {
	for (int i = 0; room->players[i]; i++) {
		if (!strcmp(room->players[i], clt->name)) {
			free(room->players[i]);
			for (int k = i; room->players[k]; k++)
				room->players[k] = room->players[k + 1];
			room->players = realloc(room->players, sizeof(char *) * (1 + oe_strarr_len(room->players)));
			break;
		}
	}
	srv->room_libs[room->type].leave(room, clt);
	clt->room_name[0] = '\0';
	if (!room->players[0]) {
		srv_free_room(room, srv);
		oe_hashmap_remove(&srv->rooms, room->name, NULL);
		free(room);
	}
}

int srv_on_room_join(room_info_t *room, client_t *clt, server_t *srv) {
	if (srv->room_libs[room->type].join(room, clt))
		return 1;
	room->players = oe_strarr_append(room->players, strdup(clt->name), NULL, 0);
	strcpy(clt->room_name, room->name);
	return 0;
}
