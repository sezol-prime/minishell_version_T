#include "minishell.h"
#include <unistd.h>

static void	hd_loop(const char *delim, int wfd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		/* 只读取“灯”的状态；不在这里写业务状态 */
		if (g_signal == SIGINT)
		{
			free(line);
			break ;
		}
		write(wfd, line, ft_strlen(line));
		write(wfd, "\n", 1);
		free(line);
	}
}

int	ms_heredoc_build(t_ms *ms, const char *delim)
{
	int	fds[2];

	if (pipe(fds) == -1)
		return (-1);
	ms->heredoc_sigint = 0;   /* 进入前清零 */
	g_signal = 0;             /* 清灯，避免上次残留 */

	ms_setup_signals_heredoc();
	hd_loop(delim, fds[1]);
	/* 结束后立刻恢复交互信号，避免影响后续 readline 行为 */
	ms_setup_signals_interactive();

	close(fds[1]);

	if (g_signal == SIGINT)
	{
		/* 在普通上下文里“落盘”业务状态与 $?，这样是合规的 */
		ms->heredoc_sigint = 1;
		g_signal = 0;           /* 清灯 */
		set_status(ms, 130);    /* $? = 130 */
		close(fds[0]);
		return (-2);            /* 维持你原来的“被中断”返回码语义 */
	}
	return (fds[0]);           /* 读端交给调用者 */
}
