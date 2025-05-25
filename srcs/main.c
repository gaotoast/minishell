#include "minishell.h"

volatile sig_atomic_t	g_sig_received;

int	main(int argc, char **argv, char **envp)
{
	int		ret;
	t_shell	*shell;

	(void)argc;
	(void)argv;
	rl_event_hook = event;
	g_sig_received = 0;
	if (init(&shell, envp) < 0)
		exit(1);
	while (1)
	{
		init_signals();
		shell->input = readline("minishell$ ");
		if (!shell->input)
			break ;
		if (*shell->input)
			add_history(shell->input);
		ret = interpret(shell);
		if (ret < 0)
			break ;
		finish_loop(shell);
	}
	exit_shell(shell);
	return (0);
}
