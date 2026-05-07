#include "client.h"

void clt_free() {
	mtx_lock(&clt.lock);
	close(clt.fd);
	clt.fd = -1;
	mtx_unlock(&clt.lock);

	buffer_free(&clt.buffer);

	thrd_join(clt.thread, NULL);
	mtx_destroy(&clt.lock);
	clt.menu = NULL;
	clt.room_name[0] = '\0';
	clt.name[0] = '\0';
	json_destroy(&clt.json);
}
