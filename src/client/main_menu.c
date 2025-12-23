/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: axllers <axlleres@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 02:25:27 by axllers           #+#    #+#             */
/*   Updated: 2025/12/23 02:26:48 by axllers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	char *buttons[] = {
		"Log in",
		"Register",
		"Exit"
	};

	for (int i = 0; i < (int)ARR_LEN(buttons); i++) {
		if (state == i)
			printf("\e[7m");
		terminal_draw_str_centered(buttons[i], 8 + 2 * i, 0);
		printf("\e[0m");
	}

	fflush(stdout);
}

static void do_button(int state) {
	if (state == 0)
		clt.menu = clt.menu;
	else if (state == 1)
		clt.menu = clt_register_menu;
	else if (state == 2)
		clt.menu = NULL;
}

void clt_main_menu() {
	int width = 0;
	int height = 0;
	terminal_get_size(&width, &height);
	terminal_clear(0);
	term_set_mode_gui();

	int state = 0;
	redisplay(state);
	while (1) {
		char *input = terminal_get_input(0);
		if (!input) {
			usleep(10000);
			continue;
		}
		if (!strcmp(input, K_DOWN) || !strcmp(input, "j"))
			state = (state + 1) % 3;
		if (!strcmp(input, K_UP) || !strcmp(input, "k"))
			state = (state + 2) % 3;
		if (!strcmp(input, " ") || !strcmp(input, "\n")) {
			do_button(state);
			break;
		}
		redisplay(state);
	}
}
