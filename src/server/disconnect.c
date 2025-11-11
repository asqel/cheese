#include "server.h"

void srv_disconnect(server_t *srv, char *name) {
	printf("client %s has been disconnected\n", name);
	oe_hashmap_remove(&srv->clients, name, (void (*)(char *, void *))srv_free_client);
}
