#include "server.h"

void srv_disconnect(server_t *srv, char *name) {
	printf("client %s has been disconnected\n", name);
	client_t *clt = oe_hashmap_get(&srv->clients, name);
	if (!clt)
		return ;
	srv_free_client(clt, srv);
	free(clt);
	oe_hashmap_remove(&srv->clients, name, NULL);
}
