#include "server.h"

void srv_free_client(char *, client_info_t *clt) {
	// TODO
	close(clt->fd);
}

void srv_free_room(char *, room_info_t *) {
	// TODO
}
