#include "client.h"
#include <signal.h>

client_data_t clt = {0};

void launch_client(int argc, char **argv) {
	(void)argc;
	(void)argv;
	signal(SIGINT, SIG_IGN);
	if (init_client(argc, argv))
		return ;
	terminal_set_screen(1);
	
	terminal_set_screen(0);
}
