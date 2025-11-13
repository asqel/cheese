#include "server.h"

int srv_account_exists(char *name) {
	FILE *f = fopen(name, "r");
	if (!f)
		return 0;
	fclose(f);
	return 1;
}

int srv_auth_account(client_t *clt, void *data, uint16_t len, server_t *srv) {
	(void)clt;
	(void)data;
	(void)len;
	(void)srv;
	//TODO
	return 0;
}

int srv_create_account(client_t *clt, void *data, uint16_t len, server_t *srv) {
	(void)srv;
	size_t name_len = 0;
	while (name_len < len && ((char *)data)[name_len])
		name_len++;
	if (name_len == 0 || name_len > CLIENT_NAME_LEN) {
		uint32_t err = OPC_ERR_NAME_LEN;
		srv_send(clt, OPC_ERROR, &err, sizeof(uint32_t));
		return 0;
	}
	if (srv_account_exists((char *)data)) {
		uint32_t err = OPC_ERR_NAME_TAKEN;
		srv_send(clt, OPC_ERROR, &err, sizeof(uint32_t));
		return 0;
	}
	if (len - (name_len + 1) != 64) {
		uint32_t err = OPC_ERR_INVALID_DATA;
		srv_send(clt, OPC_ERROR, &err, sizeof(uint32_t));
		return 0;
	}
	char *name = (char *)data;
	char *hash = (char *)data + name_len + 1;

	FILE *f = fopen(name, "wb");
	if (!f) {
		srv_send(clt, OPC_SERVER_FAIL, NULL, 0);
		return 0;
	}

	fwrite(hash, 1, 64, f);
	fflush(f);
	fclose(f);
	return 0;
}
