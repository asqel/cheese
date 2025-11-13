#include "server.h"

void srv_send(client_t *clt, uint32_t opcode, void *data, uint16_t len) {
	send(clt->fd, &opcode, sizeof(uint32_t), 0);
	send(clt->fd, &len, sizeof(uint16_t), 0);
	send(clt->fd, data, len, 0);

}
