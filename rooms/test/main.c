#include "server.h"

typedef struct {
	int can_start;
	int has_started;
} test_room_t;

int init_func(room_info_t *self) {
	printf("initing test room\n");
	test_room_t *info = malloc(sizeof(test_room_t));
	if (!info) {
		uint32_t err = OPC_ERR_FAIL;
		srv_room_send_all(self, OPC_ERROR, &err, sizeof(uint32_t));
		return 1;
	}
	self->data = info;
	return 0;
}

void free_func(room_info_t *self) {
	free(self->data);
	printf("freeing test room\n");
}

int join_func(room_info_t *self, client_t *clt) {
	(void)self;
	printf("`%s' join test room\n", clt->name);
	return 0;
}

void leave_func(room_info_t *self, client_t *clt) {
	(void)self;
	printf("`%s' leave test room\n", clt->name);
}
void move_func(room_info_t *self, client_t *clt, uint32_t pos1[2], uint32_t pos2[2]) {
	(void)self;
	printf("`%s' wants to move from (%d, %d) to (%d, %d)\n",
		clt->name, pos1[0], pos1[1], pos2[0], pos2[1]);
	srv_send_success(clt, OPC_MOVE);
}

void recv_func(room_info_t *self, client_t *clt, uint32_t opcode, void *data, uint16_t len) {
	(void)self;
	(void)opcode;
	if (((char *)data)[len] || strlen(data) > 120) {
		srv_send_err(clt, OPC_ERR_INVALID_DATA);
		return ;
	}
	printf("`%s': %s\n", clt->name, (char *)data);
	if (!strcmp(data, "on"))
		((test_room_t *)self->data)->can_start = 1;
	else if (!strcmp(data, "off"))
		((test_room_t *)self->data)->can_start = 0;
	else if (!strcmp(data, "win"))
		srv_on_room_win(self, clt->name);
	else if (!strcmp(data, "reset"))
		srv_on_room_reset(self);
}

int start_func(room_info_t *self) {
	if (((test_room_t *)self->data)->can_start == 0) {
		srv_send_err(oe_hashmap_get(&srv->clients, self->players[0]), OPC_ERR_FORBIDDEN);	
		return 1;
	}
	if (((test_room_t *)self->data)->has_started) {
		srv_send_err(oe_hashmap_get(&srv->clients, self->players[0]), OPC_ERR_FORBIDDEN);	
		return 1;
	}
	((test_room_t *)self->data)->has_started = 1;
	return 0;
}

void reset_func(room_info_t *self) {
	*((test_room_t *)self->data) = (test_room_t){0};
	(void)self;
}
