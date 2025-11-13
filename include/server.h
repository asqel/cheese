#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <oeuf.h>
#include "cheese.h"
#include "opcode.h"

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
	char name[CLIENT_NAME_LEN + 1];
	buffer_t buffer;
	char room_name[ROOM_NAME_LEN + 1];
} client_t;

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
void srv_end(server_t *srv);
int srv_loop(server_t *srv);

void srv_free_client(char *, client_t *clt);
void srv_free_room(char *, room_info_t *clt);

void srv_connect(server_t *srv);
void srv_disconnect(server_t *srv, char *name);
int srv_on_read(server_t *srv, char *name, void *buffer, int len);
int srv_handle_msg(client_t *clt, uint32_t opcode, void *data, uint16_t len, server_t *srv);
void srv_send(client_t *clt, uint32_t opcode, void *data, uint16_t len);
int srv_create_account(client_t *clt, void *data, uint16_t len, server_t *srv);
int srv_auth_account(client_t *clt, void *data, uint16_t len, server_t *srv);


#endif
