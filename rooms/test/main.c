#include "server.h"

int aaa() {
	return 0;
}

int init_func(room_info_t *self) {
	(void)self;
	return 0;
}

void free_func(room_info_t *self) {
	(void)self;
}

int join_func(room_info_t *self, client_t *clt) {
	(void)clt;
	(void)self;
	return 0;
}

void leave_func(room_info_t *self, client_t *clt) {
	(void)clt;
	(void)self;
}
void move_func(room_info_t *self, client_t *clt, uint32_t pos1[2], uint32_t pos2[2]) {
	(void)self;
	(void)clt;
	(void)pos1;
	(void)pos2;
}

void recv_func(room_info_t *self, client_t *clt, uint32_t opcode, void *data, uint16_t len) {
	(void)self;
	(void)clt;
	(void)opcode;
	(void)data;
	(void)len;
}
