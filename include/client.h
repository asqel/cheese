#ifndef CLIENT_H
#define CLIENT_H

#include "cheese.h"
#include "server.h"

typedef struct {
	int fd;
	buffer_t buffer;
	char name[CLIENT_NAME_LEN + 1];
	char room_name[ROOM_NAME_LEN + 1];
	uint8_t room_type;
	void (*menu)();
} client_data_t;

extern client_data_t clt;

void clt_main_menu();
void launch_client(int argc, char **argv);
int clt_wait_kb_srv(int *is_srv, int *is_kb, int timeout); // returns 1 if to read;

#endif
