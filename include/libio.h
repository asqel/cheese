#ifndef LIBIO_H
#define LIBIO_H

typedef struct {
	uint8_t type;
} libio_event_t;

int libio_init(char *ip, int do_stdin);

int libio_get_event(libio_event_t *ev, uint32_t timemout);

void libio_exit();

#endif
