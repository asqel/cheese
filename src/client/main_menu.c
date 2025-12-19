#include "client.h"

#define BANNER_WIDTH 23

static char *menu[] ={ \
	"        ╦  ╦  ╦        \n", \
	"        ╚╧╧╩╧╧╝        \n", \
	"╔══ ╦ ╦ ╔══ ╔══ ╔══ ╔══\n", \
	"║   ╠═╣ ╠══ ╠══ ╚═╗ ╠══\n", \
	"╚══ ╩ ╩ ╚══ ╚══ ══╝ ╚══\n", \
	NULL \
};

static void redisplay(int state) {
	terminal_clear(0);
	terminal_draw_strarr_centered(menu, 1, 0);

	if (state == 0)
		printf("\e[7m");
	terminal_draw_str_centered("Log in", 8, 0);
	printf("\e[0m");

	if (state == 1)
		printf("\e[7m");
	terminal_draw_str_centered("Register", 10, 0);
	printf("\e[0m");

	if (state == 2)
		printf("\e[7m");
	terminal_draw_str_centered("Exit", 12, 0);
	printf("\e[0m");

	fflush(stdout);
}

static void do_button(int state) {
	if (state == 2)
		clt.menu = NULL;
}

void clt_main_menu() {
	int width = 0;
	int height = 0;
	terminal_get_size(&width, &height);
	terminal_clear(0);
	terminal_set_canon(0);
	terminal_set_echo(0);
	terminal_set_block(0);
	terminal_set_cursor(0);
	terminal_set_flush(0);

	int state = 0;
	redisplay(state);
	while (1) {
		char *input = terminal_get_input();
		if (!input) {
			usleep(10000);
			continue;
		}
		if (!strcmp(input, "\e[B") || !strcmp(input, "j"))
			state = (state + 1) % 3;
		if (!strcmp(input, "\e[A") || !strcmp(input, "k"))
			state = (state + 2) % 3;
		if (!strcmp(input, " ") || !strcmp(input, "\n")) {
			do_button(state);
			break;
		}
		redisplay(state);
	}
	terminal_set_flush(1);
	terminal_set_cursor(1);
}
