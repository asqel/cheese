#include "server.h"

void srv_end(server_t *srv) {
	oe_hashmap_free(&srv->clients, (void (*)(char *, void *))srv_free_client);
	oe_hashmap_free(&srv->clients, (void (*)(char *, void *))srv_free_room);
	close(srv->fd);
	srv->fd = -1;
	for (int i = 0; i < 256; i++)
		dlclose(srv->funcs[6]);
}
