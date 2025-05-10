#include "minishell.h"

int	exec_builtin_cmd(t_node *node, char **envp)
{
	int	status;

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

int	pipe_exec_builtin(t_node *node, t_exec *ctx, char **envp, int *status)
{
	int stashed_stdin;
	int stashed_stdout;
	int exec_res;

	stashed_stdin = dup(STDIN_FILENO);
	stashed_stdout = dup(STDOUT_FILENO);
	if (stashed_stdin == -1 || stashed_stdout == -1)
	{
		perror("minishell");
		ctx->builtin_status = 1;
		return (1);
	}
	exec_res = apply_redirs(node); // apply_redirsはminishell.hで宣言
	if (exec_res == 0)
		(*status) = exec_builtin_cmd(node, envp);
	if (dup2(stashed_stdin, STDIN_FILENO) == -1)
		perror("minishell");
	if (dup2(stashed_stdout, STDOUT_FILENO) == -1)
		perror("minishell");
	close(stashed_stdin);
	close(stashed_stdout);
	ctx->child_count = 0;
	ctx->builtin_status = 0;
	return (0);
}