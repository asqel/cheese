#include "server.h"

int srv_handle_custom_opc(client_t *clt, uint8_t *data, uint16_t len, server_t *srv) {
	if (len < sizeof(uint32_t)) {
		srv_send_err(clt, OPC_ERR_INVALID_DATA);
		return 0;
	}
	if (!clt->room_name[0]) {
		srv_send_err(clt, OPC_ERR_NOT_IN_ROOM);
		return 0;
	}
	room_info_t *room = oe_hashmap_get(&srv->rooms, clt->room_name);
	void *c_data = NULL;
	if (len - 4)
		c_data = data + 4;
	srv->room_libs[room->type].recv(room, clt, *(uint32_t *)data, c_data, len - 4);
	return 0;
}
