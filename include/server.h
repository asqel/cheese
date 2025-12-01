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
	uint8_t type;
	void *data;
	char passwd_hash[65];
} room_info_t;

typedef struct {
	void *handler;
	int (*init)(room_info_t *self);
	void (*free)(room_info_t *self);
	int (*join)(room_info_t *self, client_t *clt);
	void (*leave)(room_info_t *self, client_t *clt);
	void (*move)(room_info_t *self, client_t *clt, uint32_t pos1[2], uint32_t pos2[2]);
	void (*recv)(room_info_t *self, client_t *clt, uint32_t opcode, void *data, uint16_t len);
	void (*reset)(room_info_t *self);
	int (*start)(room_info_t *self);
} room_lib_t;

typedef struct {
	int end;
	int fd;
	int port;
	char *path;
	oe_hashmap_t clients;
	oe_hashmap_t rooms;
	room_lib_t room_libs[256];
} server_t;

extern server_t *srv;

int srv_parse_args(int argc, char **argv);
int srv_start(int argc, char **argv);
void srv_end();
int srv_loop();

void srv_free_client(client_t *clt);
void srv_free_room(room_info_t *room);

void srv_connect();
void srv_disconnect(char *name);
int srv_on_read(char *name, void *buffer, int len);
int srv_handle_msg(client_t *clt, uint32_t opcode, void *data, uint16_t len);
void srv_send(client_t *clt, uint32_t opcode, void *data, uint16_t len);
void srv_send_err(client_t *clt, uint32_t err);
int srv_create_account(client_t *clt, void *data, uint16_t len);
int srv_auth_account(client_t *clt, void *data, uint16_t len);
int srv_is_name_valid(char *name);
void srv_send_success(client_t *clt, uint32_t opc);
char *srv_build_path(char *path, char *suffix);
int srv_handle_move(client_t *clt, uint8_t *data, uint16_t len);
int srv_handle_custom_opc(client_t *clt, uint8_t *data, uint16_t len);
int srv_create_room(client_t *clt, void *data, uint16_t len);
int srv_join_room(client_t *clt, void *data, uint16_t len);
int srv_on_room_join(room_info_t *room, client_t *clt);
void srv_on_room_exit(room_info_t *room, client_t *clt);
int srv_exit_room(client_t *clt, void *data, uint16_t len);
void srv_room_send_all(room_info_t *room, uint32_t opcode, void *data, uint16_t len);
void srv_on_room_reset(room_info_t *room); 
int srv_on_room_start(room_info_t *room);
int srv_start_room(client_t *clt, void *data, uint16_t len);

#endif
