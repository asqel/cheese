#include "client.h"

void clt_free() {
	mtx_lock(&clt.fd_lock);
	close(clt.fd);
	clt.fd = -1;
	mtx_unlock(&clt.fd_lock);

	buffer_free(&clt.buffer);

	thrd_join(clt.thread, NULL);
	mtx_destroy(&clt.fd_lock);
	clt.menu = NULL;
	clt.room_name[0] = '\0';
	clt.name[0] = '\0';
	json_destroy(&clt.json);
}
