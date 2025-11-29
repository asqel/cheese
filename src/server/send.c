#include "server.h"

void srv_send(client_t *clt, uint32_t opcode, void *data, uint16_t len) {
	send(clt->fd, &opcode, sizeof(uint32_t), 0);
	send(clt->fd, &len, sizeof(uint16_t), 0);
	send(clt->fd, data, len, 0);
}

void srv_send_err(client_t *clt, uint32_t err) {
	srv_send(clt, OPC_ERROR, &err, sizeof(uint32_t));
}

void srv_send_success(client_t *clt, uint32_t opc) {
	srv_send(clt, OPC_SUCCESS, &opc, sizeof(uint32_t));
}
