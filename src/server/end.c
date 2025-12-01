#include "server.h"


static void free_clt(char *name, void *data) {
	(void)name;
	srv_free_client(data);
	free(data);
}

void srv_end(server_t *srv) {
	oe_hashmap_free(&srv->clients, free_clt);
	oe_hashmap_free(&srv->rooms, NULL);
	close(srv->fd);
	srv->fd = -1;
	for (int i = 0; i < 256; i++)
		if (srv->room_libs[i].handler)
			dlclose(srv->room_libs[i].handler);
}
