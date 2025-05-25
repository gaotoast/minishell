#include "minishell.h"

int	interpret(t_shell *shell)
{
	tokenize(shell->input, &shell->tokens);
	if (sh_stat(ST_LOOK, 0) == 1)
		return (-1);
	else if (sh_stat(ST_LOOK, 0) != 0)
		return (1);
	free(shell->input);
	shell->input = NULL;
	parse(shell->tokens, &shell->ast);
	if (sh_stat(ST_LOOK, 0) == 1)
		return (-1);
	else if (sh_stat(ST_LOOK, 0) != 0)
		return (1);
	if (expand(shell->ast) != 0)
	{
		sh_stat(ST_SET, 1);
		return (-1);
	}
	execute(shell->ast);
	return (0);
}
