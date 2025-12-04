#include "server.h"
#include <signal.h>
#include <unistd.h>

static void default_init() {
	srv->port = 42124;
	srv->path = ".";
	srv->fd = -1;
}

static int init_socket() {
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
		srv_end();
		return 1;
}

int init_srv() {
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

void sigint_ignore(int sig) {
	(void)sig;
	putc('\n', stdout);
	puts("sopping server");
	if (srv)
		srv->end = 1;
}

server_t *srv = NULL;

int srv_start(int argc, char **argv) {
	srv = calloc(1, sizeof(server_t));
	if (!srv) {
		perror("calloc");
		return 1;
	}
	default_init();
	if (srv_parse_args(argc, argv))
		return 1;
	if (init_socket())
		return 1;
	if (init_srv())
		return 1;
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, sigint_ignore);
	int ret = srv_loop();
	srv_end();
	return ret;
}
