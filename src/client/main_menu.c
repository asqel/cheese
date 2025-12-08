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
	terminal_clear();
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
	printf("%*s%slog in\e[0m\n", width / 2 - 6 / 2, "", mod1);
	printf("\n");
	printf("%*s%ssign up\e[0m\n", width / 2 - 7 / 2, "", mod2);
	fflush(stdout);
}

void main_menu_func() {
	int width = 0;
	int height = 0;
	terminal_get_size(&width, &height);
	terminal_clear();

	int state = 0;
	redisplay(state);
	while (1) {
		char buff[256];
		buff[read(0, buff, 255)] = '\0';
		if (!strcmp(buff, "\e[A") || !strcmp(buff, "\e[B"))
			state = !state;
		if (!strcmp(buff, "q"))
		
	}
	terminal_set_canon(0);
	clt.next_menu = NULL;
}
