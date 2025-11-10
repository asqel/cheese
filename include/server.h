#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <oeuf.h>
#include "cheese.h"

/*
server args:
	-p PORT: set the port (default 42124)
	--path PATH: set the PWD (default ./)
*/


#define CLIENT_NAME_LEN 20
#define ROOM_NAME_LEN 20

#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__)

typedef struct {
	int fd;
	int is_registered;
	char name[CLIENT_NAME_LEN + 1];
	buffer_t buffer;
	char room_name[ROOM_NAME_LEN + 1];
} client_info_t;

typedef struct room_info_t {
	char name[ROOM_NAME_LEN];
	char **players;
	size_t players_len;
	uint8_t type;
	void *data;
	int (*on_recv)( // returns 1 to be destroyed
		struct room_info_t *self,
		char *src_name,
		uint16_t op,
		uint16_t len,
		void *data
	);
	int (*on_disconect)( // returns 1 to be destroyed
		struct room_info_t *self,
		char *src_name
	);
	int (*on_join)( // returns 1 to deny entry
		struct room_info_t *self,
		char *src_name,
		char *passwd_hash
	);
} room_info_t;

typedef struct {
	int fd;
	int port;
	char *path;
	oe_hashmap_t clients;
	oe_hashmap_t rooms;
} server_t;

int srv_parse_args(int argc, char **argv, server_t *srv);
int srv_start(int argc, char **argv);

#endif
