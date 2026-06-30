#include "libio.h"

static void send_ping() {
	uint8_t msg[6] = {0};
	uint32_t opcode = OPC_PING;
	memcpy(msg, &opcode, 4);
	mtx_lock(&g_libio_to_send_lock);
	buffer_append(&g_libio_to_send, msg, 6);
	mtx_unlock(&g_libio_to_send_lock);
}
