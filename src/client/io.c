#include "client.h"

static void send_ping() {
	mtx_lock(&clt.lock);
	if (clt.fd == -1) {
		mtx_unlock(&clt.lock);
		return ;
	}
	uint8_t msg[6] = {
		((uint32_t)OPC_PING) & 0xff,
		((uint32_t)OPC_PING >> 8) & 0xff,
		((uint32_t)OPC_PING >> 16) & 0xff,
		((uint32_t)OPC_PING >> 24) & 0xff,
		0,
		0
	};
	send(clt.fd, msg, 6, 0);

	mtx_unlock(&clt.lock);
}

static int can_read(int fd) {
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.revents = 0;
	pfd.events = POLLIN;
	poll(&pfd, 1, 0);
	return pfd.revents & POLLIN;
}

static void parse_event(uint32_t opcode, uint16_t len, uint8_t *data) {
	if (opcode == OPC_SUCCESS && len == 4) {	
		clt.next_event.type = CLT_EVENT_SUCCESS;
		clt.next_event.success = 0;
		clt.next_event.success |= (uint32_t)data[0];
		clt.next_event.success |= (uint32_t)data[1] << 8;
		clt.next_event.success |= (uint32_t)data[2] << 16;
		clt.next_event.success |= (uint32_t)data[3] << 24;
	}
	else if (opcode == OPC_ERROR && len == 4) {
		clt.next_event.type = CLT_EVENT_ERROR;
		clt.next_event.error = 0;
		clt.next_event.error |= (uint32_t)data[0];
		clt.next_event.error |= (uint32_t)data[1] << 8;
		clt.next_event.error |= (uint32_t)data[2] << 16;
		clt.next_event.error |= (uint32_t)data[3] << 24;
	}
	else {
		clt.next_event.type = opcode + CLT_EVENT_OPCODE;
		clt.next_event.data = malloc(len);
		memcpy(clt.next_event.data, data, len);
	}
}

static uint32_t read_u32(void *data, int off) {
	uint8_t *ptr = data;
	ptr += off;
	uint32_t res = 0;
	for (int i = 0; i < 4; i++) {
		uint32_t tmp = ptr[i];
		tmp <<= i * 8;
		res |= tmp;
	}
	return res;
}

static uint16_t read_u16(void *data, int off) {
	uint8_t *ptr = data;
	ptr += off;
	uint16_t res = 0;
	for (int i = 0; i < 2; i++) {
		uint16_t tmp = ptr[i];
		tmp <<= i * 8;
		res |= tmp;
	}
	return res;
}

static int build_event() {
	if (clt.buffer.len < 6)
		return 0;
	uint32_t opcode = read_u32(clt.buffer.data, 0); 

	uint16_t len = read_u16(clt.buffer.data, 4);
	if (clt.buffer.len - 6 < len)
		return 0;
	if (opcode == OPC_PING) {
		buffer_remove(&clt.buffer, 0, len + 6);	
		return 1;
	}

	uint8_t *data = (uint8_t *)clt.buffer.data + 6;
	parse_event(opcode, len, data);

	buffer_remove(&clt.buffer, 0, len + 6);	
	return 0;
}

static int update_events() {
	mtx_lock(&clt.lock);
	if (clt.fd < 0 || clt.next_event.type != CLT_EVENT_NONE) {
		mtx_unlock(&clt.lock);
		return 0;
	}
	if (!can_read(clt.fd)) {
		mtx_unlock(&clt.lock);
		return 0;
	}
	static uint8_t buff[4096];
	int len = recv(clt.fd, buff, 4096, 0);
	if (len <= 0) {
		clt.fd = -1;
		mtx_unlock(&clt.lock);
		return 1;
	}
	buffer_append(&clt.buffer, buff, len);
	while (build_event())
		;

	mtx_unlock(&clt.lock);
	return 0;
}

int clt_recv_thread(void *arg) {
	(void)arg;

	time_t next_ping = time(NULL);
	while (1) {
		if (time(NULL) >= next_ping) {
			next_ping = time(NULL) + 25;
			send_ping();
		}
		if (update_events())
			break;
		usleep(1000 * 5);
	}
	return 0;
}

void clt_get_event(client_event_t *ev) {
	if (!ev)
		return ;
	mtx_lock(&clt.lock);
	if (clt.fd != -1) {
		*ev = clt.next_event;
		clt.next_event.type = CLT_EVENT_NONE;
	}
	else
		ev->type = CLT_EVENT_DISCONNECT;
	mtx_unlock(&clt.lock);
}

void clt_event_free(client_event_t *ev) {
	if (ev->type >= CLT_EVENT_OPCODE)
		free(ev->data);
}
