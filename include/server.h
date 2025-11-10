#ifndef SERVER_H
#define SERVER_H

#include <oeuf.h>
#include "cheese.h"

#define CLIENT_NAME_LEN 20

typedef struct {
	int fd;
	int is_registered;
	char name[CLIENT_NAME_LEN + 1];
	buffer_t buffer;
} client_info_t;

typedef struct {
	int fd;
	int port;
	char *path;
	oe_hashmap_t clients;
} server_t;

void server_start(int argc, char **argv);

#endif
