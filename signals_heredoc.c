#include "minishell.h"
#include <signal.h>
#include <unistd.h>

/* 注意：不要在本文件重复定义 g_signal —— 定义在 signals.c 中 */

static void	sigint_heredoc(int sig)
{
	(void)sig;
	g_signal = SIGINT;                /* 只点灯 */
	write(STDOUT_FILENO, "\n", 1);    /* async-signal-safe */
}

void	ms_setup_signals_heredoc(void)
{
	signal(SIGINT, sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
}
