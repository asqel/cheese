#include "server.h"
#include <unistd.h>

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

	if (bind(srv->fd, (void *)&addr, sizeof(addr)) < 0)
		goto err;

	int opt = 1;
	if (setsockopt(srv->fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		goto err;
	
	if (listen(srv->fd, 256) < 0)
		goto err;

	return 0;
	err:
		perror("cheese");
		srv_end(srv);
		return 1;
}

int init_srv(server_t *srv) {
	if (oe_hashmap_init(&srv->clients, 4096)) {
		PRINT_ERR("cheese: allocation error for clients\n");
		return 1;
	}
	if (oe_hashmap_init(&srv->rooms, 4096)) {
		oe_hashmap_free(&srv->clients, NULL);
		PRINT_ERR("cheese: allocation error for rooms\n");
		return 1;
	}
	return 0;
}

int srv_start(int argc, char **argv) {
	server_t srv = (server_t){0};
	default_init(&srv);
	if (srv_parse_args(argc, argv, &srv))
		return 1;
	if (init_socket(&srv))
		return 1;
	if (init_srv(&srv))
		return 1;
	if (srv.path)
		chdir(srv.path);
	int ret = srv_loop(&srv);
	srv_end(&srv);
	return ret;
}
