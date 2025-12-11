#include "client.h"
#include <netdb.h>

static char *get_host(json_value_t *json) {
	json_value_t *value = json_object_get(json, "host");
	if (!value || value->type != JSON_STRING)
		return "asqel.ddns.net";

	return value->val.str;
}

static char *get_port(json_value_t *json) {
	json_value_t *value = json_object_get(json, "port");
	if (!value || value->type != JSON_NUMBER)
		return "42124";

	static char res[256];
	sprintf(res, "%d", (int)value->val.num);
	return res;
}

static int init_socket(json_value_t *json) {
	char *host = get_host(json);
	char *port = get_port(json);
	printf("trying to connect to %s:%s\n", host, port);

	struct addrinfo hints;
	struct addrinfo *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(host, port, &hints, &res)) {
		perror("getaddrinfo");
		return 1;
	}

	int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sock < 0) {
		perror("socket");
		freeaddrinfo(res);
		return 1;
	}

	if (connect(sock, res->ai_addr, res->ai_addrlen) < 0) {
		perror("connect");
		freeaddrinfo(res);
		close(sock);
		return 1;
	}
	freeaddrinfo(res);
	clt.fd = sock;
	return 0;
}

int clt_init(int argc, char **argv) {
	if (argc > 1) {
		PRINT_ERR("Error: client takes at most 1 argument\n");
		return 1;
	}

	json_value_t json = (json_value_t){0};
	if (argc == 1 && json_from_file(argv[0], &json)) {
		return 1;
	}
	if (init_socket(&json)) {
		json_destroy(&json);
		return 1;
	}
	json_destroy(&json);

	mtx_init(&clt.fd_lock, mtx_plain);
	thrd_create(&clt.thread, clt_ping_thread, NULL);
	return 0;
}
