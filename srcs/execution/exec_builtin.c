#include "minishell.h"

int	exec_builtin_cmd(t_node *node, char **envp)
{
	// char	*cmd;

	(void)node;
	(void)envp;
	// cmd = node->argv[0];
	// if (ft_strncmp(cmd, "echo", 5) == 0)
	// 	sh_stat(echo(node->argc, node->argv), ST_SET);
	// else if (ft_strncmp(cmd, "cd", 3) == 0)
	// 	sh_stat(cd(node->argc, node->argv), ST_SET);
	// else if (ft_strncmp(cmd, "pwd", 4) == 0)
	// 	sh_stat(pwd(node->argc, node->argv));
	// else if (ft_strncmp(cmd, "export", 7) == 0)
	// 	sh_stat(export(node->argc, node->argv), ST_SET);
	// else if (ft_strncmp(cmd, "unset", 6) == 0)
	// 	sh_stat(unset(node->argc, node->argv), ST_SET);
	// else if (ft_strncmp(cmd, "env", 4) == 0)
	// 	sh_stat(env(node->argc, node->argv), ST_SET);
	// else if (ft_strncmp(cmd, "exit", 5) == 0)
	// 	sh_stat(exit_builtin(node->argc, node->argv), ST_SET);
	return (sh_stat(0, ST_GET));
}

int	pipe_exec_builtin(t_node *node, t_exec *ctx, char **envp)
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
	exec_res = apply_redirs(node);
	if (exec_res == 0)
		exec_builtin_cmd(node, envp);
	if (dup2(stashed_stdin, STDIN_FILENO) == -1)
    {
		perror("minishell");
        return (1);
    }
	if (dup2(stashed_stdout, STDOUT_FILENO) == -1)
    {
		perror("minishell");
        return (1);
    }
	close(stashed_stdin);
	close(stashed_stdout);
	ctx->child_count = 0;
	ctx->builtin_status = 0;
	return (0);
}
