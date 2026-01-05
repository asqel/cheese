#include "client.h"

/*

   			   Register
		
		name: ...
		password: ...
		confirm password: ...
		
		cancel
		
		errors
		
		

*/

void redisplay(int state, char name[CLIENT_NAME_LEN + 1], char *error) {
	int width = 0;
	int height = 0;

	terminal_get_size(&width, &height);
	terminal_clear(0);
	terminal_draw_str_centered("Register", 2, 0);

	int y = 5;
	int x = width / 2 - (strlen("name") + 2 + CLIENT_NAME_LEN) / 2;
	
	char *prefixes[5] = {"", "", "", "", ""};
	if (state < 5)
		prefixes[state] = "\e[7m";

	terminal_goto(x, y++, 0);
	printf("%sname: %s\e[0m", prefixes[0], name);

	terminal_goto(x, y++, 0);
	printf("%spassword:\e[0m", prefixes[1]);

	terminal_goto(x, y++, 0);
	printf("%sconfirm password:\e[0m", prefixes[2]);

	printf("\e[0;91m");
	terminal_draw_str_centered(error, y + 1, 0);
	printf("\e[0m");

	int total_len = 3 + strlen("confirm") + strlen("cancel");
	x = width / 2 - total_len / 2; 	
	terminal_goto(x, y + 3, 0);
	printf("%sconfirm\e[0m   %scancel\e[0m", prefixes[3], prefixes[4]);


	fflush(stdout);	
}

void clt_register_menu() {	
	terminal_clear(0);
	term_set_mode_gui();

	int state = 0;
	char name[CLIENT_NAME_LEN + 1] = {0};
	char error[256] = {0};
	strcpy(error, "sample error");

	redisplay(state, name, error);
	while (1) {
		char *input = terminal_get_input(0);
		if (!input) {
			usleep(10000);
			continue;
		}
		if (!strcmp(input, K_DOWN))
			state = (state + 1) % 5;
		else if (!strcmp(input, K_UP))
			state = (state + 4) % 5; 
		else if (state >= 3) {
			if (!strcmp(input, K_LEFT) || !strcmp(input, K_RIGHT)
				|| !strcmp(input, "h") || !strcmp(input, "l")) {
				state -= (state == 4) * 2 - 1;
			}
		}
		else if (state == 0) {
			int is_good = 1;
			for (int i = 0; input[i] && is_good; i++) {
				if (input[i] < ' ' && input[i] > 0)
					is_good = 0;
			}
			if (!is_good && strcmp(input, "\b"))
				continue;


		}
		redisplay(state, name, error);
	}
	clt.menu = clt_main_menu;
}
