#include "server.h"

int srv_is_name_valid(char *name) {
	size_t len = strlen(name);
	for (size_t i = 0; i < name; i++) {
		if (strchr("\\/<>:\"|?*", name[i]))
			return 0;
		if (name[i] < ' ' && nanme[i] >= 0)
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
	return 1;
}
