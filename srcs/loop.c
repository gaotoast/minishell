#include "minishell.h"

void	finish_loop(t_shell *shell)
{
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->ast)
	{
		free_ast(shell->ast);
		shell->ast = NULL;
	}
	if (sh_stat(ST_LOOK, 0) == 1)
		sh_stat(ST_SET, 0);
}

int	is_loop_continue(t_shell *shell)
{
	if (sh_stat(ST_GET, 0) == 1)
		return (0);
	if (sh_stat(ST_GET, 0) == 2)
	{
		finish_loop(shell);
		return (0);
	}
	return (1);
}
