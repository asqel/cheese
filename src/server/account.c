#include "server.h"

int srv_account_exists(char *name) {
	FILE *f = fopen(name, "r");
	if (!f)
		return 0;
	fclose(f);
	return 1;
}

int srv_account_check_hash(char *name, char *hash) {
	FILE *f = fopen(name, "r");
	if (!f)
		return 0;
	char file_hash[64];
	if (fread(file_hash, 1, 64, f) < 64) {
		fclose(f);
		return 0;
	}
	fclose(f);
	return !memcmp(file_hash, hash, 64);
}

static int get_infos(void *data, uint16_t len, char **name, char **hash, client_t *clt) {
	size_t name_len = 0;
	while (name_len < len && ((char *)data)[name_len])
		name_len++;
	if (name_len == 0 || name_len > CLIENT_NAME_LEN) {
		srv_send_err(clt, OPC_ERR_NAME_LEN);
		return 1;
	}
	if (len - (name_len + 1) != 64) {
		srv_send_err(clt, OPC_ERR_INVALID_DATA);
		return 1;
	}
	if (!srv_is_name_valid(data)) {
		srv_send_err(clt, OPC_ERR_NAME_INVALID);
		return 1;
	}
	*name = (char *)data;
	*hash = (char *)data + name_len + 1;
	return 0;
}

int srv_auth_account(client_t *clt, void *data, uint16_t len, server_t *srv) {
	char *name = NULL;
	char *hash = NULL;
	if (clt->name[0] != '=') {
		srv_send_err(clt, OPC_ERR_ALREADY_AUTH);
		return 0;
	}
	if (get_infos(data, len, &name, &hash, clt))
		return 0;
	if (!srv_account_check_hash(name, hash)) {
		srv_send_err(clt, OPC_ERR_WRONG_PASSW);
		return 0;
	}
	srv_disconnect(srv, name);
	oe_hashmap_remove(&srv->clients, clt->name, NULL);
	oe_hashmap_set(&srv->clients, name, clt);
	strcpy(clt->name, name);
	srv_send_success(clt, OPC_AUTH_ACC);
	return 1;
}

int srv_create_account(client_t *clt, void *data, uint16_t len, server_t *srv) {
	(void)srv;
	char *name = NULL;
	char *hash = NULL;
	if (get_infos(data, len, &name, &hash, clt))
		return 0;
	
	if (srv_account_exists((char *)data)) {
		srv_send_err(clt, OPC_ERR_NAME_TAKEN);
		return 1;
	}

	FILE *f = fopen(name, "wb");
	if (!f) {
		srv_send_err(clt, OPC_ERR_FAIL);
		return 0;
	}

	fwrite(hash, 1, 64, f);
	fflush(f);
	fclose(f);
	srv_send_success(clt, OPC_CREATE_ACC);
	return 0;
}
