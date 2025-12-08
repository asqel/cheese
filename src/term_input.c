#include "cheese.h"

static void process_stash(char *res, char *s, char *s_len) {
	if (s[0] <= 0x7f || (s[0] == '\e' && s_len == 1)) {
		res[0] = s[0];
		res[1] = '\0';
		s_len--;
		memcpy(s, s + 1, s_len);
		return res;
	}
	if (s[0] == '\e') {
		if (s[1] == '[' && s_len > 2) {
			
		}
		else if (s[1] == 'O' && s_len > 2) {
		
		}
		else {
			res[0] = s[0];
			res[1] = s[1];
			res[2] = '\0';
			s_len -= 2;
			memcpy(s, s + 2, s_len);
			return res;
		}
	}
}

char *terminal_get_input() {
	static char res[256];
	static char stash[4096];
	static int stash_len;

	while (stash_len < sizeof(stash)) {	
		int ret = read(0, &stash[stash_len], sizeof(stash) - stash_len);
		if (ret <= 0)
			break;
		stash_len += ret;
	}
	if (!stash_len)
		return NULL;
	return process_stash(res, stash, stash_len);
}
