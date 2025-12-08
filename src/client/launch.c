#include "client.h"
#include <signal.h>

client_data_t clt = {0};

static int init_client(int argc, char **argv) {
	(void)argc;
	(void)argv;
	return 0;
}

void launch_client(int argc, char **argv) {
	(void)argc;
	(void)argv;
	signal(SIGINT, SIG_IGN);
	if (init_client(argc, argv))
		return ;
	save_tty();
	terminal_set_screen(1);
	clt.next_menu = main_menu_func;
	terminal_set_ctrl_key('C', 0);
	terminal_set_ctrl_key('D', 0);
	terminal_set_ctrl_key('\\', 0);
	while (clt.next_menu)
		clt.next_menu();
	terminal_set_screen(0);
	restore_tty();
}
