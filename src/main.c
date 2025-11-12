#include "cheese.h"
#include "server.h"
#include <signal.h>

void lexit(void) {
	printf("\e[?1049l");
	fflush(stdout);
}

void on_sigint(int _) {
	(void)_;
	exit(1);
}

int	main(int argc, char **argv) {
	board_t		board = {0};
	
	if (argc > 1) {
		char *hash = sha256(argv[1]);
		printf("%s\n", hash);
		free(hash);
	}
	printf("\e[?1049h");
	fflush(stdout);
	atexit(lexit);
	signal(SIGINT, on_sigint);
	//srv_start(0, NULL);
	init_board("base", &board);
	while (1) {
		write(1, "\033[2J\033[H", 7);
		if (play(&board))
			break ;
	}
	free_board(board.copy_board, 0);
	free_board(&board, 1);
	free(board.copy_board);
	
	//getc(stdin);
	return (0);
}
