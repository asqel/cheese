#include "server.h"

void srv_connect() {
	static uint64_t last = 0;

	client_t *clt = calloc(1, sizeof(client_t));
	clt->fd = accept(srv->fd, NULL, NULL);

	int tries = 30;
	while (tries--) {
		sprintf(clt->name, "=%lx", last++);
		if (oe_hashmap_get(&srv->clients, clt->name))
			continue;
		oe_hashmap_set(&srv->clients, clt->name, clt); 
		printf("new client %s\n", clt->name);
		return ;
	}
	close(clt->fd);
	free(clt);
	printf("too much temporary clients\n");
}
