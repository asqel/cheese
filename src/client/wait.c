#include "client.h"

int clt_wait_kb_srv(int *is_srv, int *is_kb, int timeout) {
	if (is_srv)
		*is_srv = 0;
	if (is_kb)
		*is_kb = 0;

	struct pollfd fds[2] = {{0}};

	fds[0].fd = clt.fd;
	fds[1].fd = 0;
	fds[0].events = POLIN;
	fds[1].events = POLIN;

	int count = poll(fds, 2, timeout);
	if (count < 0)
		return -1;
	if (is_srv && (fds[0].revents & POLIN))
		*is_srv = 1;
	if (is_kb && (fds[0].revents & POLIN))
		*is_kb = 1;

	return count;
}
