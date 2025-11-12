#include "server.h"

int srv_handle_msg(client_info_t *clt, uint32_t opcode, void *data, uint16_t len, server_t *srv) {	
	 switch (ocpode) {
	 	case OPC_CREATE_ACC:
			return srv_create_account(clt, data, len, srv);
		case OPC_AUTH_ACC:
			return srv_auth_account(clt, data, len, srv);
	 	default:
			srv_send(clt, OPC_UNKNOW_OP, &opcode, sizeof(uint32_t), srv);
			break;
	 }
	 return 0;
}

int srv_on_read(server_t *srv, char *name, void *data, int len) {
	client_info_t *clt = oe_hashmap_get(&srv->clients, name);
	if (!clt)
		return ;
	buffer_append(&clt->buffer, data, len);
	if (clt->buffer.len < sizeof(uint32_t) + sizeof(uint16_t))
		return ;
	uint32_t opcode = 0;
	uint16_t pack_len = 0;
	memcpy(&opcode, clt->buffer.data, sizeof(uint32_t));
	memcpy(&pack_len, clt->buffer.data + sizeof(uint32_t), sizeof(uint16_t));

	if (clt->buffer.len < sizeof(uint32_t) + sizeof(uint16_t) + pack_len)
		return ;

	void *packet = clt->buffer.data + sizeof(uint32_t) + sizeof(uint16_t);
	srv_handle_msg(clt, opcode, packet, pack_len, srv);
}
