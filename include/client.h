#ifndef CLIENT_H
#define CLIENT_H

#include "cheese.h"
#include "server.h"
#include <threads.h>

typedef struct {
	json_value_t json;
	int fd;
	buffer_t buffer;
	char name[CLIENT_NAME_LEN + 1];
	char room_name[ROOM_NAME_LEN + 1];
	uint8_t room_type;
	void (*menu)();

	mtx_t fd_lock;
	thrd_t thread;
} client_data_t;

extern client_data_t clt;

void launch_client(int argc, char **argv);
int clt_wait_kb_srv(int *is_srv, int *is_kb, int timeout); // returns 1 if to read;
int clt_init(int argc, char **argv);
void clt_free();
int clt_ping_thread(void *arg); 

// menus
void clt_main_menu();
void clt_register_menu();

#endif
