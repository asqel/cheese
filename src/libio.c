#include "libio.h"

int g_libio_fd = -1;
uint8_t g_libio_do_stdin = 0;
buffer_t g_libio_received = {0};

mtx_t g_libio_to_send_lock = {0}
buffer_t g_libio_to_send = {0};

thrd_t g_libio_loop_thread = {0};

uint8_t g_libio_stop = 0;
mtx_t g_libio_stop_lock = {0};

int libio_init(int sock_fd, int do_stdin) {
	g_libio_fd = sock_fd;
	g_libio_do_stdin = do_stdin;
	g_libio_libio_stop = 0;
	mtx_init(&g_libio_to_send_lock);
	mtx_init(&g_libio_libio_stop_lock);
	if (thrd_create(&g_libio_loop_thread, libio_loop, NULL) != thrd_success) {
		g_libio_fd = -1;
		g_libio_do_stdin = 0;
		return 1;
	}
	return 0;
}

void libio_exit() {

}
