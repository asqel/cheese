#include "server.h"

void srv_free_client(char *name, client_t *clt) {
	// TODO
	(void)name;
	close(clt->fd);
}

void srv_free_room(char *name, room_info_t *room) {
	(void)name;
	(void)room;
	// TODO
}
