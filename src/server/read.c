#include "server.h"

int srv_handle_msg(client_t *clt, uint32_t opcode, void *data, uint16_t len) {	
	printf("client `%s': opcode %u, len %hu\n", clt->name, opcode, len);
	if (clt->name[0] == '=' && opcode != OPC_CREATE_ACC && opcode != OPC_AUTH_ACC && opcode != OPC_PING) {
		srv_send_err(clt, OPC_ERR_FORBIDDEN);
		return 0;	
	}
	switch (opcode) {
		case OPC_CREATE_ACC:
			return srv_create_account(clt, data, len);
		case OPC_AUTH_ACC:
			return srv_auth_account(clt, data, len);
		case OPC_PING:
			srv_send(clt, OPC_PING, data, len);
			return 0;
		case OPC_MOVE:
			return srv_handle_move(clt, data, len);
		case OPC_CUSTOM:
			return srv_handle_custom_opc(clt, data, len);
		case OPC_CREATE_ROOM:
			return srv_create_room(clt, data, len);
		case OPC_JOIN:
			return srv_join_room(clt, data, len);
		case OPC_EXIT_ROOM:
			return srv_exit_room(clt, data, len);
		case OPC_ASK_START:
			return srv_start_room(clt, data, len);
		case OPC_ASK_BOARD:
			return srv_ask_board(clt, data, len);
	 	default:
			srv_send(clt, OPC_UNKNOW_OP, &opcode, sizeof(uint32_t));
			break;
	 }
	 return 0;
}

int srv_on_read(char *name, void *data, int len) {
	client_t *clt = oe_hashmap_get(&srv->clients, name);
	if (!clt)
		return 0;
	buffer_append(&clt->buffer, data, len);
	int ret = 0;
	while (1) {
		if (clt->buffer.len < sizeof(uint32_t) + sizeof(uint16_t))
			break;
		uint32_t opcode = 0;
		uint16_t pack_len = 0;
		memcpy(&opcode, clt->buffer.data, sizeof(uint32_t));
		memcpy(&pack_len, clt->buffer.data + sizeof(uint32_t), sizeof(uint16_t));

		if (clt->buffer.len < sizeof(uint32_t) + sizeof(uint16_t) + pack_len)
			break;

		void *packet = clt->buffer.data + sizeof(uint32_t) + sizeof(uint16_t);
		ret |= srv_handle_msg(clt, opcode, packet, pack_len);
		buffer_remove(&clt->buffer, 0, sizeof(uint32_t) + sizeof(uint16_t) + pack_len);
	}
	return ret;
}
