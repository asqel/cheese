#include "cheese.h"

static void process_stash(char *res, uint8_t *s, int s_len) {
	int to_read = 0;
	if ((s[0] <= 0x7f && s[0] != '\e') || (s[0] == '\e' && s_len == 1))
		to_read = 1;
	else if (s[0] == '\e') {
		if (s[1] == '[' && s_len > 2) {
			int i = 2;				
			while (i < s_len && 0x30 <= s[i] && s[i] <= 0x3f)
				i++;
			while (i < s_len && 0x20 <= s[i] && s[i] <= 0x2f)
				i++;
			while (i < s_len && 0x40 <= s[i] && s[i] <= 0x7e)
				i++;
			to_read = i;
		}
		else if (s[1] == 'O' && s_len > 2)
			to_read = 3;
		else
			to_read = 2;
	}
	else {
		if (s[0] >> 5 == 0b110 && s_len >= 2)
			to_read = 2;
		else if (s[0] >> 4 == 0b1110 && s_len >= 3)
			to_read = 3;
		else if (s[0] >> 3 == 0b11110 && s_len >= 4)
			to_read = 4;
		else
			to_read = 1;
	}
	memcpy(res, s, to_read);
	res[to_read] = '\0';
	s_len -= to_read;
	memcpy(s, s + to_read, s_len);
}

char *terminal_get_input() {
	static char res[256];
	static char stash[4096];
	static int stash_len;

	while (stash_len < (int)sizeof(stash)) {	
		int ret = read(0, &stash[stash_len], sizeof(stash) - stash_len);
		if (ret <= 0)
			break;
		stash_len += ret;
		usleep(1000);
	}
	if (!stash_len)
		return NULL;
	process_stash(res, (uint8_t *)stash, stash_len);
	stash_len -= strlen(res);
	return res;
}
