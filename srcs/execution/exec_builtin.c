#include "minishell.h"

int	exec_builtin(t_node *node, char **envp)
{
	int		status;
	// char	*cmd;

	// cmd = node->argv[0];
	status = 1;
    (void)node;
    (void)envp;
	// if (ft_strncmp(cmd, "echo", 5) == 0)
	// 	status = echo(node->argc, node->argv);
	// else if (ft_strncmp(cmd, "cd", 3) == 0)
	// 	status = cd(node->argc, node->argv);
	// else if (ft_strncmp(cmd, "pwd", 4) == 0)
	// 	status = pwd(node->argc, node->argv);
	// else if (ft_strncmp(cmd, "export", 7) == 0)
	// 	status = export(node->argc, node->argv);
	// else if (ft_strncmp(cmd, "unset", 6) == 0)
	// 	status = unset(node->argc, node->argv);
	// else if (ft_strncmp(cmd, "env", 4) == 0)
	// 	status = env(node->argc, node->argv);
	// else if (ft_strncmp(cmd, "exit", 5) == 0)
	// 	status = exit_builtin(node->argc, node->argv);
	return (status);
}
