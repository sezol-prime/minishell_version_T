#include "minishell.h"

static void	preload_env(t_ms *ms, char **envp)
{
	ms->env = env_from(envp);
	ms->last_status = 0;
	ms->heredoc_sigint = 0;
}

int	main(int argc, char **argv, char **envp)
{
	t_ms		ms;
	extern int	rl_catch_signals;
	extern int	rl_catch_sigwinch;

	(void)argv;
	if (argc != 1)
		return (1);
	preload_env(&ms, envp);

	/* 让 readline 不自己接管信号，由我们统一处理 */
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;

	ms_setup_signals_interactive();
	return (ms_loop(&ms));
}
