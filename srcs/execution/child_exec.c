#include "minishell.h"

void	child_exec(t_node *node, char **envp)
{
	// if (apply_redirs(node->redir_count, node->redirs) != 0)
	// 	exit (1);
	if (!node->argv)
		exit(0);
	if (is_builtin(node->argv[0]))
	{
		exec_builtin_cmd(node, envp);
		exit(sh_stat(ST_GET, 0));
	}
	else
		exec_cmd(node->argv, envp);
}
