#include "libio.h"
#if defined(__linux__) || defined(__APPLE__)

static void append_disco() {
	libio_event_t ev = {0};
	ev.type = LIBIO_EV_DISCONNECT;
	libio_append_event(&ev);
}

void libio_wait_event(int timemout) {
	if (timeout < 0)
		return ;
	struct pollfd fds[3] = {0};
	fds[0].fd = libio_ctx.fd;
	fds[0].events = POLLIN;
	if (libio_ctx.to_send.len)
		fds[0].events |= POLLOUT;
	fds[1].fd = 0;
	fds[1].events = POLLIN;

	int ret = poll(fds, 3, timeout);

	uint8_t buffer[4096];
	if (fds[0].revents & POLLIN) {
		int red = recv(fds[0].fd, buffer, 4096, 0);
		if (red <= 0) {
			close(libio_ctx.fd);
			libio_ctx.fd = -1;
			libio_event_t ev = {0};
			ev.type = LIBIO_EV_DISCONNECT;
			libio_append_event(&ev);
		}
		else
			buffer_append(&libio_ctx.recv_stash, buffer, red);
	}
	if (libio_ctx.fd <= 0 && (fds[0].revents & POLLOUT) && libio_ctx.to_send.len) {
		size_t to_send_size = 1024;
		if (libio_ctx.to_send.len < to_send_size)
			to_send_size = libio_ctx.to_send.len
		int sent = send(fds[0].fd, buffer, to_send_size, MSG_NOSIGNAL);
		buffer_remove(&libio_ctx.to_send, 0, sent);
	}

	if (fds[1].revents & POLLIN) {
		int red = read(fds[1].fd, buffer, 4096);
		if (red > 0)
			buffer_append(&libio_ctx.input_stash, buffer, red);
	}
}

#endif
