#include "server.h" 

int srv_ask_board(client_t *clt, void *data, uint16_t data_len) {
	(void)data;
	if (data_len) {
		srv_send_err(clt, OPC_ERR_INVALID_DATA);
		return 0;
	}
	if (!clt->room_name[0]) {
		srv_send_err(clt, OPC_ERR_NOT_IN_ROOM);
		return 0;
	}
	room_info_t *room = oe_hashmap_get(&srv->rooms, clt->room_name);
	srv->room_libs[room->type].send_board(clt, room);
	return 0;
}
