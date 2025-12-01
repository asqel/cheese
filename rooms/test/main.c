#include "server.h"

int init_func(room_info_t *self) {
	(void)self;
	printf("initing test room\n");
	return 0;
}

void free_func(room_info_t *self) {
	(void)self;
	printf("freeing test room\n");
}

int join_func(room_info_t *self, client_t *clt) {
	(void)clt;
	(void)self;
	printf("`%s' join test room\n", clt->name);
	return 0;
}

void leave_func(room_info_t *self, client_t *clt) {
	(void)clt;
	(void)self;
	printf("`%s' leave test room\n", clt->name);
}
void move_func(room_info_t *self, client_t *clt, uint32_t pos1[2], uint32_t pos2[2]) {
	(void)self;
	(void)clt;
	(void)pos1;
	(void)pos2;
	printf("`%s' wants to move from (%d, %d) to (%d, %d)\n",
		clt->name, pos1[0], pos1[1], pos2[0], pos2[1]);
}

void recv_func(room_info_t *self, client_t *clt, uint32_t opcode, void *data, uint16_t len) {
	(void)self;
	(void)clt;
	(void)opcode;
	(void)data;
	(void)len;
}

int start_func(room_info_t *self) {
	(void)self;
	return 0;
}

void reset_func(room_info_t *self) {
	(void)self;
	return 0;
}
