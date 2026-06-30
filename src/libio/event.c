#include "libio.h"

libio_ctx_t libio_ctx = {0};

static int get_timeout(uint64_t when) {
	uint64_t now = cheese_get_time();
	if (now >= when)
		return 0;
	return (when - time);
}

static void send_ping() {
	uint8_t msg[6] = {0};
	uint32_t opcode = OPC_PING;
	memcpy(msg, &opcode, 4);
	buffer_append(&libio_ctx.to_send, msg, 6);
}

void libio_free_event(libio_event_t *ev) {
	if (ev->type == LIBIO_KB) 
		free(ev->kb);
	if (ev->type == LIBIO_EV_PACKET)
		free(ev->packet.data);
	ev->type = LIBIO_EV_NONE;
}

static int treat_recv(libio_event_t *ev) {
	buffer_t *buffer = &libio_ctx->recv_stash;
	if (buffer->len < 6)
		return 0;
	uint32_t opcode = buffer->data[0];
	opcode |= ((uint32_t)buffer->data[1]) << 8;
	opcode |= ((uint32_t)buffer->data[2]) << 16;
	opcode |= ((uint32_t)buffer->data[3]) << 24;
	uint16_t len = buffer->data[4] | (((uint16_t)buffer->data[5]) << 8);
	if (buffer->len < 6 + len)
		return 0;
	ev->type = LIBIO_EV_PACKET;
	ev->packet.data = malloc(len);
	memcpy(ev->packet.data, buffer->data, len);
	ev->packet.opcode = opcode;
	ev->packet.len = len;
	buffer_remove(&buffer, 0, len);
	return 1;
}

static void treat_event(libio_event_t *ev) {	
	if (treat_recv(ev))
		return ;
	if (libio_treat_kb(ev))
		return ;
	ev->type = LIBIO_EV_NONE;
}

void libio_get_event(libio_event_t *ev, uint64_t timeout_ms) {
	uint64_t now = cheese_get_time();
	uint64_t end = now + timeout;
	ev->type = LIBIO_EV_NONE;
	while (now < end && ev->type == LIBIO_EV_NONE) {
		treat_event(ev);

		uint64_t ping_timeout = get_timeout(next_ping);
		uint64_t user_timeout = end - now;
		uint64_t real_timeout = ping_timeout;

		if (ping_timeout > user_timeout)
			real_timeout = user_timeout;

		libio_wait_event(real_timeout);	
		now = cheese_get_time();
		if (now >= ping_timeout)
			libio_send_ping();
	}
}

