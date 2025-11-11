#include <unistd.h>
#include <termios.h>

static struct termios	*saved_config(void)
{
	static struct termios	term;
	static int				initialized = 0;

	if (!initialized)
	{
		tcgetattr(STDIN_FILENO, &term);
		initialized = 1;
	}
	return (&term);
}

void	enable_raw_mode(void)
{
	struct termios	raw;

	if (!isatty(STDIN_FILENO))
		return ;
	saved_config();
	tcgetattr(STDIN_FILENO, &raw);
	raw.c_lflag &= ~(ECHO | ICANON);
	raw.c_cc[VINTR] = 0;
	raw.c_cc[VQUIT] = 0;
	raw.c_cc[VSUSP] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void	disable_raw_mode(void)
{
	if (!isatty(STDIN_FILENO))
		return ;
	tcsetattr(STDIN_FILENO, TCSANOW, saved_config());
}

char	read_char(void)
{
	char	c;

	enable_raw_mode();
	read(0, &c, 1);
	disable_raw_mode();
	return (c);
}
