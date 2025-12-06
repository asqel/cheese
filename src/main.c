#include "cheese.h"
#include "server.h"
#include <signal.h>

static void lexit(void) {
	printf("\e[?1049l");
	fflush(stdout);
}

static void on_sigint(int _) {
	(void)_;
	exit(1);
}

void launch_gabriel(int argc, char **argv) {
	(void)argc;
	(void)argv;
	atexit(lexit);
	signal(SIGINT, on_sigint);
	printf("\e[?1049h");
	fflush(stdout);

	// do your stuff
	board_t		board = {0};
	init_board("base", &board);
	while (1) {
		write(1, "\033[2J\033[H", 7);
		if (play(&board))
			break ;
	}
	free_board(board.copy_board, 0);
	free_board(&board, 1);
	free(board.logs);
	free(board.copy_board);
	get_piece(-1);
}

int	main(int argc, char **argv) {
	if (argc == 1) {
		launch_gabriel(argc - 2, argv - 2);
		return 0;
	}
	if (!strcmp(argv[1], "-c")) {
		launch_client(argc - 2, argv - 2);
		return 0;
	}
	else if (!strcmp(argv[1], "-s")) {
		int ret = srv_start(argc - 2, argv + 2);
		free(srv);
		return ret;
	}
	else if (!strcmp(argv[1], "-g")) {
		fprintf(stderr, "AH lol\n");
		char *passwd = read_passwd();
		printf("passwd %s|\n", passwd);
		explicit_bzero(passwd, strlen(passwd) + 1);
		free(passwd);
		return 1;
	}
	fprintf(stderr, "Error: expected `-s' or `-c' as first argument\n");
	return 1;
}
