#include "cheese.h"
#include "server.h"
#include  "client.h"
#include <signal.h>

void *libpiss_handler;
config_file_t g_config_file;

static void lexit(void) {
	printf("\e[?1049l");
	fflush(stdout);
}

static void on_sigint(int _) {
	(void)_;
	exit(1);
}

static void load_libpiss(void)
{
	libpiss_handler = dlopen(LIBPISS, RTLD_LAZY);
	if (!libpiss_handler) {
		fprintf(stderr, "Error: Lib `%s' not found\n", LIBPISS);
		exit(1);
	}
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
	for (int i = 0; g_config_file.piece_types[i]; i++)
		free(g_config_file.piece_types[i]);
	free(g_config_file.piece_types);

	dlclose(libpiss_handler);
	for (int i = 0; i < board.nb_piece; i++) {
		oe_strarr_free(board.pieces[i]->possible_locations, board.height);
		free_possible_moves(&board, board.pieces[i]->possible_moves);
		free(board.pieces[i]);
	}
	free_possible_moves(&board, board.default_moves);
	oe_strarr_free(board.default_locations, board.height);
	free_board(&board);
	free(board.logs);
	free(board.pieces);
	free(board.players);
}

int	main(int argc, char **argv) {
	//return (0);
	if (argc == 1) {
		load_libpiss();
		parse_config_file("config_template.yml");
		launch_gabriel(0, NULL);
		return 0;
	}
	if (!strcmp(argv[1], "-c")) {
		launch_client(argc - 2, argv + 2);
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
