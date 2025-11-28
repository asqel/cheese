#include "server.h"

static server_t *g_srv = NULL;

static void free_clt(char *name, void *data) {
	(void)name;
	srv_free_client(data, g_srv);
	free(data);
}

void srv_end(server_t *srv) {
	g_srv = srv;
	oe_hashmap_free(&srv->clients, free_clt);
	g_srv = NULL;
	oe_hashmap_free(&srv->rooms, NULL);
	close(srv->fd);
	srv->fd = -1;
}
