#include "minishell.h"
#include <signal.h>
#include <string.h>
#include <unistd.h>

/* 唯一定义处：唯一全局变量，仅用于传递“收到的信号编号” */
volatile sig_atomic_t	g_signal = 0;

/* 交互态：handler 里只“点灯”+ 写换行；UI/状态更新放到主循环里做 */
static void	sigint_interactive(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1); /* async-signal-safe */
}

static void	sigquit_ignore(int sig)
{
	(void)sig;
}

void	ms_setup_signals_interactive(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigint_interactive;
	sigemptyset(&sa.sa_mask);
	/* 不设置 SA_RESTART，使得 read/readline 能被 EINTR 打断返回 */
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);

	signal(SIGQUIT, sigquit_ignore);
}

void	ms_setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
