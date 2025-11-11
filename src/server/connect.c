#include "server.h"

void srv_connect(server_t *srv) {
	static int last = 0;
	client_info_t *clt = calloc(1, sizeof(client_info_t));
	clt->fd = accept(srv->fd, NULL, NULL);
	sprintf(clt->name, "==%d", last++);
	oe_hashmap_set(&srv->clients, clt->name, clt); 
}
