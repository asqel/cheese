#include "server.h"

int srv_is_name_valid(char *name) {
	if (name[0] == '=')
		return 0;
	size_t len = strlen(name);
	size_t i = 0;
	for (i = 0; i < len; i++) {
		if (name[i] == ' ' && name[i + 1] == ' ')
			return 0;
		if (strchr("\\/<>:\"|?*", name[i]))
			return 0;
		if (name[i] < ' ' && name[i] >= 0)
			return 0;
		if (name[i] >> 5 == 0b110) {
			i++;
			continue;
		}
		if (name[i] >> 4 == 0b1110) {
			i += 2;
			continue;
		}
		if (name[i] >> 3 == 0b11110) {
			i += 3;
			continue;
		}
		if (name[i] >= 0x7f)
			return 0;
	}
	return (i == len);
}

char *srv_build_path(char *path, char *suffix) {
	char *res = malloc(strlen(path) + 2 + strlen(srv->path) + strlen(suffix));
	sprintf(res, "%s/%s%s", srv->path, path, suffix);
	return res;
}
