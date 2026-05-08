#include "libio.h"

static int g_fd = -1;
static buffer_t g_received = {0};
static buffer_t g_to_send = {0};
static thrd_t loop_thread = {0};

int libio_init(char *ip, int do_stdin) {
	
	return 0;
}
