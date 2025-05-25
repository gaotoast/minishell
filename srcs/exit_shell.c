#include "minishell.h"

// シェルを終了
void	exit_shell(t_shell *shell)
{
	rl_clear_history();
	ft_env(ENV_DEL_ALL, NULL);
	free_shell(shell);
	ft_exit(1, NULL);
}
