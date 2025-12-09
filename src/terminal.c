#include "cheese.h"
#include <sys/ioctl.h>
#include <ctype.h>

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

int terminal_set_ctrl_key(char key, int state) {
	struct termios tty;
	tcgetattr(0, &tty);
	int old_state = 0;

	switch (key) {
		case 'C':
			old_state = tty.c_cc[VINTR];
			tty.c_cc[VINTR] = state ? 0x03 : _POSIX_VDISABLE;
			break;
		case '\\':
			old_state = tty.c_cc[VQUIT];
			tty.c_cc[VQUIT] = state ? 0x1c : _POSIX_VDISABLE;
			break;
		case 'D':
			old_state = tty.c_cc[VEOF];
			tty.c_cc[VEOF] = state ? 0x04 : _POSIX_VDISABLE;
			break;
	}
	tcsetattr(0, TCSAFLUSH, &tty);
	return old_state;
}

void terminal_get_pos(int *x, int *y) {
	tcflush(0, TCIFLUSH);
	write(1, "\e[6n", 4);
	char c = 0;
	int row = 0;
	int col = 0;
	while (1) {
		read(0, &c, 1);
		if (c != '\e')
			continue;
		read(0, &c, 1);
		if (c != '[')
			continue;
		read(0, &c, 1);
		if (!isdigit(c))
			continue;
		row = 0;
		col = 0;
		while (isdigit(c)) {
			row = row * 10 + c - '0';
			read(0, &c, 1);
		}
		if (c != ';')
			continue;
		read(0, &c, 1);
		if (!isdigit(c))
			continue;
		while (isdigit(c)) {
			col = col * 10 + c - '0';
			read(0, &c, 1);
		}
		if (c != 'R')
			continue;
		break;
	}
	*x = col;
	*y = row;
}

void terminal_get_size(int *w, int *h) {
	struct winsize win;
	ioctl(0, TIOCGWINSZ, &win);

	*w = win.ws_col;
	*h = win.ws_row;
}

void terminal_clear() {
	write(1, "\e[2J\e[H", 7);
}

int terminal_set_block(int state) {
	int flags = fcntl(0, F_GETFL, 0);
	if (state)
		fcntl(0, F_SETFL, flags & ~(O_NONBLOCK));
	else
		fcntl(0, F_SETFL, flags | O_NONBLOCK);
	return flags;
}

void terminal_set_cursor(int state) {
	if (state)
		printf("\e[?25h");
	else
		printf("\e[?25l");
	fflush(stdout);
}
