#include "server.h"

void srv_disconnect(char *name) {
	printf("client %s has been disconnected\n", name);
	client_t *clt = oe_hashmap_get(&srv->clients, name);
	if (!clt)
		return ;
	srv_free_client(clt);
	free(clt);
	oe_hashmap_remove(&srv->clients, name, NULL);
}
