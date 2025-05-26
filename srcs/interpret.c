#include "minishell.h"

void	exit_shell(t_shell *shell)
{
	rl_clear_history();
	ft_env(ENV_DEL_ALL, NULL);
	free_shell(shell);
	ft_exit(1, NULL);
}

int	handle_stage_ret(t_shell *shell, int ret)
{
	if (ret == 1)
	{
        sh_stat(ST_SET, 1);
		exit_shell(shell);
		return (1);
	}
	else if (ret != 0)
	{
		sh_stat(ST_SET, ret);
		return (ret);
	}
	return (0);
}

void	interpret(t_shell *shell)
{
	int	ret;

	ret = tokenize(shell->input, &shell->tokens);
	if (handle_stage_ret(shell, ret) != 0)
		return ;
	free(shell->input);
	shell->input = NULL;
	ret = parse(shell->tokens, &shell->ast);
	if (handle_stage_ret(shell, ret) != 0)
		return ;
	ret = expand(shell->ast);
	if (handle_stage_ret(shell, ret) != 0)
		return ;
	ret = execute(shell->ast);
	if (handle_stage_ret(shell, ret) != 0)
		return ;
}
