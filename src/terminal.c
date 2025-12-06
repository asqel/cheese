#include "cheese.h"

static int old_stdin_flags = 0;
static struct termios old_tty = {0};
static int is_tty_saved = 0;

void save_tty() {
	if (is_tty_saved)
		return ;
	old_stdin_flags = fcntl(0, F_GETFL, 0);
	tcgetattr(0, &old_tty);
	is_tty_saved = 1;
}

void restore_tty() {
	if (!is_tty_saved)
		return ;
	fcntl(0, F_SETFL, old_stdin_flags);
	tcsetattr(0, TCSAFLUSH, &old_tty);
	is_tty_saved = 0;
}

int terminal_set_echo(int enable) {
	struct termios tty;
	tcgetattr(0, &tty);
	int old_state = tty.c_lflag & ECHO;
	if (enable) 
		tty.c_lflag |= ECHO;
	else
		tty.c_lflag &= ~(ECHO);
	tcsetattr(0, TCSAFLUSH, &tty);
	return old_state;
}

int terminal_set_canon(int enable) {
	struct termios tty;
	tcgetattr(0, &tty);
	int old_state = tty.c_lflag & ICANON;
	if (enable) 
		tty.c_lflag |= ICANON;
	else
		tty.c_lflag &= ~(ICANON);
	tcsetattr(0, TCSAFLUSH, &tty);
	return old_state;
}

char *read_passwd() {
	int old_echo = terminal_set_echo(0);
	char *str = NULL;
	size_t len = 0;
	size_t alloc_len = 0;
	while (1) {
		char c;
		int ret = read(0, &c, 1);
		if (ret <= 0)
			exit(1);
		if (alloc_len == len) {
			alloc_len += 64;
			char *new_str = malloc(alloc_len);
			memcpy(new_str, str, len);
			explicit_bzero(str, len);
			free(str);
			str = new_str;
		}
		str[len++] = c;
		if (c == '\n')
			break;
	}
	str[len - 1] = '\0';	
	terminal_set_echo(old_echo);
	return str;
}

void terminal_set_screen(int is_alt) {
	if (is_alt)
		printf("\e[?1049h");
	else
		printf("\e[?1049l");
	fflush(stdout);
}
