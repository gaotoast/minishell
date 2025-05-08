#include "minishell.h"

typedef struct s_pipe
{
	int pipe_read;
	int child_count;
	int child_pid[128];
}	t_pipe;

void execute_pipeline(t_node *node, t_pipe *ctx, char **envp)
{
	int pipefd[2];
	pid_t pid;

	if (!node)
		return ;
	if (node->kind == ND_PIPE)
	{
		pipe(pipefd);
		execute_pipeline(node->lhs, ctx, envp);
		pid = fork();
		if (pid == 0)
		{
			if (ctx->pipe_read != -1)
			{
				dup2(ctx->pipe_read, STDIN_FILENO);
				close(ctx->pipe_read);
			}
			dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[1]);
			close(pipefd[0]);
			// redirect
			execute_pipeline(node->rhs, ctx, envp);
		}
		close(pipefd[1]);
		ctx->child_pid[ctx->child_count++] = pid;
		if (ctx->pipe_read != -1)
			close(ctx->pipe_read);
		ctx->pipe_read = pipefd[0];
	}
	// else if (node->kind == ND_CMD)
	// {
	// 	// pid = fork();
	// 	// if (pid == 0)
	// 	// {
	// 	// 	if (ctx->state == PIPE_READ)
	// 	// 	{
	// 	// 		dup2(ctx->pipe_read, STDIN_FILENO);
	// 	// 		close(ctx->pipe_read);
	// 	// 	}
	// 	// 	// redirect
	// 	// 	exec_cmd(node->argv, envp);
	// 	// }
	// 	// ctx->child_pid[ctx->child_count++] = pid;
	// }
}

void wait_all_children(t_pipe ctx, int *status)
{
	int i;

	i = 0;
	while (i < ctx.child_count)
	{
		waitpid(ctx.child_pid[i], status, 0);
		i++;
	}
	if (ctx.pipe_read != -1)
		close(ctx.pipe_read);
}

int execute(t_node *root, char **envp)
{
	t_pipe ctx;
	int status;

	ctx.pipe_read = -1;
	ctx.child_count = 0;
	execute_pipeline(root, &ctx, envp);
	wait_all_children(ctx, &status);
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
