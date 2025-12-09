#include "client.h"

#define BANNER_WIDTH 23

static char *menu[] ={ \
	"%*s        ╦  ╦  ╦        \n", \
	"%*s        ╚╧╧╩╧╧╝        \n", \
	"%*s╔══ ╦ ╦ ╔══ ╔══ ╔══ ╔══\n", \
	"%*s║   ╠═╣ ╠══ ╠══ ╚═╗ ╠══\n", \
	"%*s╚══ ╩ ╩ ╚══ ╚══ ══╝ ╚══\n", \
	NULL \
};

static void redisplay(int state) {
	terminal_clear(0);
	int width = 0;
	int height = 0;
	terminal_get_size(&width, &height);
	
	for (int i = 0; menu[i]; i++)
		printf(menu[i], width / 2 - BANNER_WIDTH / 2, "");

	char *mod1 = "";
	char *mod2 = "";
	if (state == 0)
		mod1 = "\e[7m";
	else
		mod2 = "\e[7m";
	printf("\n");
	printf("\n");
	printf("\n");
	printf("%*s%sLog in\e[0m\n", width / 2 - 6 / 2, "", mod1);
	printf("\n");
	printf("%*s%sSign up\e[0m\n", width / 2 - 7 / 2, "", mod2);
	fflush(stdout);
}

void clt_main_menu() {
	int width = 0;
	int height = 0;
	terminal_get_size(&width, &height);
	terminal_clear();
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
		if (!strcmp(input, "q"))
			break;
		if (!strcmp(input, "\e[B") || !strcmp(input, "\e[A"))
			state = !state;
		redisplay(state);
	}
	terminal_set_flush(1);
	terminal_set_cursor(1);
}
