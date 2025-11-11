#include "server.h"

void srv_on_read(server_t *srv, char *name, void *data, int len) {
	//TODO
	(void)srv;
	printf("%s: %.*s\n", name, len, (char *)data);
}
