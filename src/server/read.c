#include "server.h"

void srv_on_read(server_t *, char *name, void *data, int len) {
	//TODO
	printf("%s: %.*s\n", name, len, data);
}
