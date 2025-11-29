#include "server.h"

int srv_handle_move(client_t *clt, uint8_t *data, uint16_t len, server_t *srv) {
	if (len != sizeof(uint32_t) * 4) {
		srv_send_err(clt, OPC_ERR_INVALID_DATA);
		return 0;
	}
	if (!clt->room_name[0]) {
		srv_send_err(clt, OPC_ERR_NOT_IN_ROOM);
		return 0;
	}
	room_info_t *room = oe_hashmap_get(&srv->rooms, clt->room_name);
	srv->room_libs[room->type].move(room, clt, (uint32_t *)data, (uint32_t *)data + 2);
	return 0;
}
