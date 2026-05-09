#include "libio.h"

static int g_fd = -1;
static uint8_t g_do_stdin = 0;
static buffer_t g_received = {0};
static buffer_t g_to_send = {0};
static thrd_t g_loop_thread = {0};

static uint8_t g_libio_stop = 0;
static mtx_t g_libio_stop_lock = {0};

void *libio_loop(void *arg) {
	(void)arg;
	while (1) {
		// !TODO check if poll == -1 later
	}
	return NULL;
}

int libio_init(int sock_fd, int do_stdin) {
	g_fd = sock_fd;
	g_do_stdin = do_stdin;
	signal(SIGUSR1, libio_sighandler);
	if (thrd_create(&g_loop_thread, libio_loop, NULL) != thrd_success) {
		g_fd = -1;
		g_do_stdin = 0;
		return 1;
	}
	return 0;
}

void libio_exit() {

}
