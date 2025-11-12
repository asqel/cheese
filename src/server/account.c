#include "server.h"

int srv_create_account(client_t *clt, void *data, size_t len, server_t *srv ) {
	size_t name_len = 0;
	while (name_len < size_t && data[name_len])
		name_len++;
	if (name == 0 || name > CLIENT_NAME_LEN) {
		srv_send(clt, OPC_ERROR, 
		return 0;
	}
	if (srv_account_exists())
		
}
