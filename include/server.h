#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <dlfcn.h>
#include <poll.h>
#include <oeuf.h>
#include "cheese.h"
#include "opcode.h"
#include "room.h"
#include "onion.h"

/*
server args:
	-p PORT: set the port (default 42124)
	--path PATH: set the PWD (default ./)
*/


#define CLIENT_NAME_LEN 20
#define ROOM_NAME_LEN 40

#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__)

typedef struct {
	int fd;
	char name[CLIENT_NAME_LEN + 1];
	buffer_t buffer;
	char room_name[ROOM_NAME_LEN + 1];
} client_t;

typedef struct room_info_t {
	char name[ROOM_NAME_LEN + 1];	
	char **players;
	char host[CLIENT_NAME_LEN + 1];
	uint8_t type;
	void *data;
	char passwd_hash[65];
} room_info_t;

typedef struct {
	void *handler;
	int (*init)(room_info_t *self);
	void (*free)(room_info_t *self);
	int (*join)(room_info_t *self, client_t *clt);
	int (*leave)(room_info_t *self, client_t *clt);
	void (*mov)(room_info_t *self, client_t *clt, uint32_t pos1[2], uint32_t pos2[2]);
	void (*recv_custom)(room_info_t *self, client_t *clt, uint32_t opcode, void *data, uint16_t len);
} room_lib_t;

typedef struct {
	int fd;
	int port;
	char *path;
	oe_hashmap_t clients;
	oe_hashmap_t rooms;
	room_lib_t room_libs[256];
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
void srv_send_err(client_t *clt, uint32_t err);
int srv_create_account(client_t *clt, void *data, uint16_t len, server_t *srv);
int srv_auth_account(client_t *clt, void *data, uint16_t len, server_t *srv);
int srv_is_name_valid(char *name);
void srv_send_success(client_t *clt, uint32_t opc);
char *srv_build_path(server_t *srv, char *path, char *suffix);

#endif
