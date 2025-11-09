#ifndef SERVER_H
#define SERVER_H

#include <oeuf.h>

#define CLIENT_NAME_LEN 20

typedef struct {
	int fd;
	char name[CLIENT_NAME_LEN + 1];
} client_info_t;

typedef struct {
	int fd;
	int port;
	oe_hashmap_t clients;
} server_t;

void server_start(int argc, char **argv);

#endif
