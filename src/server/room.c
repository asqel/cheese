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
	// TODO check if already in room
	uint8_t room_type = *((uint8_t *)data + len - 1);
	// TODO check room type
	room_info_t *room = malloc(sizeof(room_info_t));
	memset(room, 0, sizeof(room_info_t));
	strcpy(room->name, name);
	oe_strarr_append(&room->players, clt->name, NULL, 0);
	strcpy(room->host, clt->name);
	memcpy(room->passwd_hash, hash, 64);

	
	// TODO send success
	return 0;
}
