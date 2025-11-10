#include "server.h"

static void default_init(server_t *srv) {
	srv->port = 42124;
	srv->path = NULL;
	srv->fd = -1;
}

static int init_socket(server_t *srv) {
	srv->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (srv->fd < 0)
		goto err;
	
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(srv->port);

	if (bind(srv->fd, (void *)addr, sizeof(addr)) < 0)

	return 0;
	err:
		perror("cheese");
		server_end(srv);
		return 1;
}

int srv_start(int argc, char **argv) {
	server_t srv = (server_t){0};
	default_init(&srv);
	if (server_parse_args(argc, argv, &srv))
		return 1;
	if (init_socket(&srv))
		return 1;
	if (init_srv(&srv))
		return 1;
	int ret = srv_loop(&srv);
	srv_end(&srv);
	return ret;
}
