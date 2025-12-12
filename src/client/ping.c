#include "client.h"
#include <time.h>

int clt_ping_thread(void *arg) {
	(void)arg;
	uint8_t msg[6] = {0};
	uint32_t opcode = OPC_PING;
	memcpy(msg, &opcode, 4);

	time_t next_ping = time(NULL);
	while (1) {
		mtx_lock(&clt.fd_lock);
		if (clt.fd == -1)
			break;
		if (time(NULL) >= next_ping) {
			send(clt.fd, msg, sizeof(msg), 0);
			next_ping = time(NULL) + 25;
		}
		mtx_unlock(&clt.fd_lock);
		usleep(1000 * 100);	
	}
	mtx_unlock(&clt.fd_lock);
	return 0;
}
