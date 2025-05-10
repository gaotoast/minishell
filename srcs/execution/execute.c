#include "minishell.h"

void	exec_pipe(t_node *node, t_exec *ctx, char **envp, int in_fd, int out_fd)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("minishell");
		close_fds(in_fd, out_fd);
		return ;
	}
	execute_segment(node->lhs, ctx, envp, in_fd, pipefd[1]);
	close(pipefd[1]);
	execute_segment(node->rhs, ctx, envp, pipefd[0], out_fd);
	close(pipefd[0]);
}

void	execute_segment(t_node *node, t_exec *ctx, char **envp, int in_fd,
		int out_fd)
{
	pid_t	pid;

	if (!node)
	{
		close_fds(in_fd, out_fd);
		return ;
	}
	if (node->kind == ND_PIPE)
		exec_pipe(node, ctx, envp, in_fd, out_fd);
	else if (node->kind == ND_CMD)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("minishell");
			close_fds(in_fd, out_fd);
			return ;
		}
		if (pid == 0)
			pipe_exec_cmd(node, envp, in_fd, out_fd);
		ctx->child_pids[ctx->child_count++] = pid;
		close_fds(in_fd, out_fd);
	}
}

void	execute_pipeline(t_node *root, t_exec *ctx, char **envp, int *status)
{
	if (root->kind == ND_CMD && is_builtin(root->argv[0]))
	{
		pipe_exec_builtin(root, ctx, envp, status);
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
		return (0);
	status = 0;
	ctx.child_count = 0;
	execute_pipeline(root, &ctx, envp, &status);
	wait_children(ctx, &status);
	return (status);
}
