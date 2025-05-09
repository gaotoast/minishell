#include "minishell.h"

#define TMP "heredoc_tmp_"

typedef struct s_exec
{
	int	child_count;
	int	child_pids[128];
	int	builtin_status;
}		t_exec;

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

int	exec_builtin(t_node *node, char **envp)
{
	int		status;
	char	*cmd;

	cmd = node->argv[0];
	status = 1;
	if (ft_strncmp(cmd, "echo", 5) == 0)
		status = echo(node->argc, node->argv);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		status = cd(node->argc, node->argv);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		status = pwd(node->argc, node->argv);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		status = export(node->argc, node->argv);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		status = unset(node->argc, node->argv);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		status = env(node->argc, node->argv);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		status = exit_builtin(node->argc, node->argv);
	return (status);
}

int handle_heredoc(t_redir *redir, int index)
{
	char *index_str;
	char *temp_file;
	char *line;
	int temp_fd;
	int read_fd;

	index_str = ft_itoa(index);
	if (!index_str)
		return (-1);
	temp_file = ft_strjoin(TMP, index_str);
	if (!temp_file)
	{
		free(index_str);
		return (-1);
	}
	free(index_str);
	if (ft_strlen(TMP) > PATH_MAX)
	{
		free(temp_file);
		return (-1);
	}
	temp_fd = open(temp_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (temp_fd < 0)
	{
		free(temp_file);
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strncmp(line, redir->str, ft_strlen(redir->str)) == 0)
		{
			free(line);
			break;
		}
		write(temp_fd, line, ft_strlen(line));
	}
	close(temp_fd);
	read_fd = open(temp_file, O_RDONLY);
	if (read_fd < 0)
	{
		free(temp_file);
		unlink(temp_file);
		return (-1);
	}
	unlink(temp_file);
	return (read_fd);
}

int apply_redirs(t_node *node)
{
	int fd;
	int i;
	t_redir *redir;

	i = 0;
	while (i < node->redir_count)
	{
		redir = node->redirs[i];
		if (redir->kind == REDIR_IN)
		{
			fd = open(redir->str, O_RDONLY);
			if (fd < 0)
			{
				perror("minishell");
				return (1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->kind == REDIR_OUT)
		{
			fd = open(redir->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd < 0)
			{
				perror("minihshell");
				return (1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->kind == REDIR_APPEND)
		{
			fd = open(redir->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd < 0)
			{
				perror("minishell");
				return (1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (redir->kind == REDIR_HEREDOC)
		{
			fd = handle_heredoc(redir, i);
			if (fd < 0)
				return (1);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		i++;
	}
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
				dup2(in_fd, STDIN_FILENO) == -1;
				close(in_fd);
			}
			if (out_fd != STDOUT_FILENO && out_fd != -1)
			{
				dup2(out_fd, STDOUT_FILENO);
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

void	execute_pipeline(t_node *root, t_exec *ctx, char **envp)
{
	int	stashed_stdin;
	int	stashed_stdout;
	int	status;

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
		status = 1;
		if (apply_redirs(root) == 0)
			status = exec_builtin(root, envp);
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

	status = 0;
	ctx.child_count = 0;
	execute_pipeline(root, &ctx, envp);
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
