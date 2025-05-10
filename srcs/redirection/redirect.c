#include "minishell.h"

int	apply_heredoc_redir(t_redir *redir, int index, int *last_heredoc_fd,
		int *cur_fd)
{
	if (*last_heredoc_fd != -1)
		close(*last_heredoc_fd);
	*last_heredoc_fd = handle_heredoc(redir, index);
	if (*last_heredoc_fd < 0)
	{
		perror("minishell");
		return (1);
	}
	*cur_fd = *last_heredoc_fd;
	return (0);
}

int	apply_io_redir(t_redir *redir, int *cur_fd, int flags, int std_fd)
{
	*cur_fd = open(redir->str, flags, 0644);
	if (*cur_fd < 0)
	{
		perror("minishell");
		return (1);
	}
	if (dup2(*cur_fd, std_fd) == -1)
	{
		close(*cur_fd);
		perror("minishell");
		return (1);
	}
	close(*cur_fd);
	return (0);
}

int	process_redir(t_redir *redir, int index, int *last_heredoc_fd, int *cur_fd)
{
	if (redir->kind == REDIR_IN)
		return (apply_io_redir(redir, cur_fd, O_RDONLY, STDIN_FILENO));
	else if (redir->kind == REDIR_OUT)
		return (apply_io_redir(redir, cur_fd, O_WRONLY | O_CREAT | O_TRUNC,
				STDOUT_FILENO));
	else if (redir->kind == REDIR_APPEND)
		return (apply_io_redir(redir, cur_fd, O_WRONLY | O_CREAT | O_APPEND,
				STDOUT_FILENO));
	else if (redir->kind == REDIR_HEREDOC)
		return (apply_heredoc_redir(redir, index, last_heredoc_fd, cur_fd));
	return (0);
}

int	finish_heredoc_redir(int last_heredoc_fd)
{
	if (last_heredoc_fd != -1)
	{
		if (dup2(last_heredoc_fd, STDIN_FILENO) == -1)
		{
			close(last_heredoc_fd);
			perror("minishell");
			return (1);
		}
		close(last_heredoc_fd);
	}
	return (0);
}

int	apply_redirs(t_node *node)
{
	int	cur_fd;
	int	i;
	int	last_heredoc_fd;

	cur_fd = -1;
	i = 0;
	last_heredoc_fd = -1;
	while (i < node->redir_count)
	{
		if (cur_fd > 0)
			close(cur_fd);
		if (process_redir(node->redirs[i], i, &last_heredoc_fd, &cur_fd) != 0)
		{
			if (last_heredoc_fd != -1)
				close(last_heredoc_fd);
			return (1);
		}
		i++;
	}
	if (finish_heredoc_redir(last_heredoc_fd) != 0)
		return (1);
	return (0);
}
