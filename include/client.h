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
	void (*next_menu)();
	void *data;
} client_data_t;

extern client_data_t clt;

#endif
