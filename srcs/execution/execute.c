#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

void	execute_segment(t_node *node, t_exec *ctx, char **envp, int in_fd,
		int out_fd)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (!node)
	{
		if (in_fd != STDIN_FILENO && in_fd != -1)
			close(in_fd);
		if (out_fd != STDOUT_FILENO && out_fd != -1)
			close(out_fd);
		return ;
	}
	if (node->kind == ND_PIPE)
	{
		if (pipe(pipefd) == -1)
		{
			perror("minishell");
			if (in_fd != STDIN_FILENO && in_fd != -1)
				close(in_fd);
			if (out_fd != STDOUT_FILENO && out_fd != -1)
				close(out_fd);
			return ;
		}
		execute_segment(node->lhs, ctx, envp, in_fd, pipefd[1]);
		close(pipefd[1]);
		execute_segment(node->rhs, ctx, envp, pipefd[0], out_fd);
		close(pipefd[0]);
	}
	else if (node->kind == ND_CMD)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("minishell");
			if (in_fd != STDIN_FILENO && in_fd != -1)
				close(in_fd);
			if (out_fd != STDOUT_FILENO && out_fd != -1)
				close(out_fd);
			return ;
		}
		if (pid == 0)
		{
			if (in_fd != STDIN_FILENO && in_fd != -1)
			{
				if (dup2(in_fd, STDIN_FILENO) == -1)
				{
					perror("minishell");
					exit(EXIT_FAILURE);
				}
				close(in_fd);
			}
			if (out_fd != STDOUT_FILENO && out_fd != -1)
			{
				if (dup2(out_fd, STDOUT_FILENO) == -1)
				{
					perror("minishell");
					exit(EXIT_FAILURE);
				}
				close(out_fd);
			}
			if (apply_redirs(node) != 0)
				exit(EXIT_FAILURE);
			if (is_builtin(node->argv[0]))
			{
				status = exec_builtin(node, envp);
				exit(status);
			}
			else
				exec_cmd(node->argv, envp);
		}
		ctx->child_pids[ctx->child_count++] = pid;
		if (in_fd != STDIN_FILENO && in_fd != -1)
			close(in_fd);
		if (out_fd != STDOUT_FILENO && out_fd != -1)
			close(out_fd);
	}
}

void	execute_pipeline(t_node *root, t_exec *ctx, char **envp, int *status)
{
	int	stashed_stdin;
	int	stashed_stdout;

	if (root->kind == ND_CMD && is_builtin(root->argv[0]))
	{
		stashed_stdin = dup(STDIN_FILENO);
		stashed_stdout = dup(STDOUT_FILENO);
		if (stashed_stdin == -1 || stashed_stdout == -1)
		{
			perror("minishell");
			ctx->builtin_status = 1;
			return ;
		}
		if (apply_redirs(root) == 0)
			(*status) = exec_builtin(root, envp);
		if (dup2(stashed_stdin, STDIN_FILENO) == -1)
			perror("minishell");
		if (dup2(stashed_stdout, STDOUT_FILENO) == -1)
			perror("minishell");
		close(stashed_stdin);
		close(stashed_stdout);
		ctx->child_count = 0;
		ctx->builtin_status = 0;
		return ;
	}
	execute_segment(root, ctx, envp, STDIN_FILENO, STDOUT_FILENO);
}

void	wait_children(t_exec ctx, int *status)
{
	int	i;

	i = 0;
	while (i < ctx.child_count)
	{
		waitpid(ctx.child_pids[i], status, 0);
		i++;
	}
}

int	execute(t_node *root, char **envp)
{
	t_exec	ctx;
	int		status;

    if (!root)
    {
        return (0);
    }
	status = 0;
	ctx.child_count = 0;
	execute_pipeline(root, &ctx, envp, &status);
	wait_children(ctx, &status);
	return (status);
}

// int	execute(char **cmds, char **envp)
// {
// 	pid_t	pid;
// 	int		status;

// 	pid = fork();
// 	if (pid < 0)
// 	{
// 		perror("minishell");
// 		free_2d_array(cmds);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (pid == 0)
// 		exec_cmd(cmds, envp);
// 	waitpid(pid, &status, 0);
// 	return (status);
// }
