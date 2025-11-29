#include "server.h"

static struct pollfd *build_pollfd(server_t *srv, int len) {
	struct pollfd *res = malloc(sizeof(struct pollfd) * (1 + len));	
	if (!res)
		return NULL;
	res[0].fd = srv->fd;
	client_t **clients = (client_t **)oe_hashmap_get_values(&srv->clients);
	for (int i = 0; i < len; i++)
		res[i + 1].fd = clients[i]->fd;
	free(clients);
	return res;
}

static void reset_fds(struct pollfd *fds, int len) {
	for (int i = 0; i < len; i++) {
		fds[i].revents = 0;
		fds[i].events = POLLIN;
	}
}

int srv_loop(server_t *srv) {
	char **keys = NULL;
	struct pollfd *fds = NULL;
	int fds_len = 0;

	while (!srv->end) {
		if (!fds) {
			keys = oe_hashmap_get_keys(&srv->clients);
			fds_len = oe_hashmap_len(&srv->clients);
			fds = build_pollfd(srv, fds_len);
			fds_len++;
		}
		if (!fds)
			break;
		reset_fds(fds, fds_len);

		int ret = poll(fds, fds_len, -1);
		if (ret < 0) {
			perror("poll");
			break;
		}
		int need_rebuild = 0;
		for (int i = 0; i < fds_len && ret; i++) {
			if (need_rebuild)
				break;
			if (!(fds[i].revents & POLLIN))
				continue;
			ret--;
			if (i == 0) {
				srv_connect(srv);
				need_rebuild = 1;	
				break;
			}
			char buffer[1024];
			int ret = recv(fds[i].fd, buffer, 1024, 0);
			if (ret <= 0) {
				need_rebuild = 1;
				srv_disconnect(srv, keys[i - 1]);
				break;
			}
			need_rebuild |= srv_on_read(srv, keys[i - 1], buffer, ret);
		}
		if (need_rebuild) {
			free(fds);
			fds = NULL;
			oe_hashmap_free_keys(keys);
			keys = NULL;
			need_rebuild = 0;
		}
	}
	oe_hashmap_free_keys(keys);
	free(fds);
	return 1;
}
