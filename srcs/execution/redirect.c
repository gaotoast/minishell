#include "minishell.h"

int	apply_redirs(t_node *node)
{
	int		fd;
	int		i;
	t_redir	*redir;
    int     last_heredoc_fd;

    fd = -1;
	i = 0;
    last_heredoc_fd = -1;
	while (i < node->redir_count)
	{
        if (fd > 0)
            close(fd);
		redir = node->redirs[i];
		if (redir->kind == REDIR_IN)
		{
			fd = open(redir->str, O_RDONLY);
			if (fd < 0)
			{
				perror("minishell");
				return (1);
			}
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				close(fd);
				perror("minishell");
				return (1);
			}
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
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				close(fd);
				perror("minishell");
				return (1);
			}
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
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				close(fd);
				perror("minishell");
				return (1);
			}
			close(fd);
		}
		else if (redir->kind == REDIR_HEREDOC)
		{
            if (last_heredoc_fd != -1)
                close(last_heredoc_fd);
			last_heredoc_fd = handle_heredoc(redir, i);
			if (last_heredoc_fd < 0)
				return (1);
			fd = last_heredoc_fd;
		}
		i++;
	}
    if (last_heredoc_fd != -1) {
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
