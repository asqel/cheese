/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   register_menu.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axllers <axlleres@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/03 02:01:21 by axllers           #+#    #+#             */
/*   Updated: 2026/01/03 02:15:09 by axllers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	
	char *prefixes[3] = {"", "", ""};
	if (state < 3)
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

	if (state == 3)
		printf("\e[7m");
	terminal_draw_str_centered("cancel", y + 3, 0);
	printf("\e[0m");


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
		if (!strcmp(input, "\e"))
			break;
		if (!strcmp(input, K_DOWN))
			state = (state + 1) % 4;
		else if (!strcmp(input, K_UP))
			state = (state + 3) % 4; 
		else if (strlen(input) + strlen(name) <= CLIENT_NAME_LEN)
			strcat(name, input);
		redisplay(state, name, error);
	}
	clt.menu = clt_main_menu;
}
