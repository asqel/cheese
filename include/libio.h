#ifndef LIBIO_H
#define LIBIO_H

#include "chthread.h"

enum {
	LIBIO_EV_NONE,
	LIBIO_EV_KB,
	LIBIO_EV_PACKET,
	LIBIO_EV_DISCONNECT,
	LIBIO_EV_MAX,
};

typedef struct {
	union {
		struct {
			uint8_t *data;
			uint32_t opcode;
			uint16_t len;
		} packet;
		uint8_t *kb;
	};
	uint8_t type;
} libio_event_t;

typedef struct {

	libio_event_t *events;
	size_t events_len;
	#if defined(_WIN32)
		#error "Windows unsupported for now " __FILE__

	#elif defined(__linux__) || defined(__APPLE__)
		int fd;

	#else
		#error "unsuported OS " __FILE__

	#endif
	buffer_t to_send;
	buffer_t input_stash;
	buffer_t recv_stash;

	uint64_t next_ping;
} libio_ctx_t;

int libio_init(char *ip, int port, int do_stdin);

int libio_get_event(libio_event_t *ev, uint32_t timemout);
void libio_free_event(libio_event_t *ev);

void libio_exit();

// Internal stuff
extern libio_ctx_t libio_ctx;
void libio_wait_event(int timemout);
void libio_append_event(libio_event_t *ev);
int libio_treat_kb(libio_event_t *ev);

#endif
