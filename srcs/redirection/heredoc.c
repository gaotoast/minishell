#include "minishell.h"

int	create_temp_file(int index, char **temp_file)
{
	char	*index_str;
	int		temp_fd;

	index_str = ft_itoa(index);
	if (!index_str)
		return (-1);
	(*temp_file) = ft_strjoin(HEREDOC_TMP, index_str);
	if (!(*temp_file))
	{
		free(index_str);
		return (-1);
	}
	free(index_str);
	if (ft_strlen(HEREDOC_TMP) > PATH_MAX)
	{
		free(*temp_file);
		return (-1);
	}
	temp_fd = open(*temp_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (temp_fd < 0)
	{
		free(*temp_file);
		return (-1);
	}
	return (temp_fd);
}

void	write_heredoc_input(int temp_fd, t_redir *redir)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, redir->str, ft_strlen(redir->str)) == 0)
		{
			free(line);
			break ;
		}
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
}

int	open_and_unlink_heredoc(char *temp_file)
{
	int	read_fd;

	read_fd = open(temp_file, O_RDONLY);
	if (read_fd < 0)
	{
		unlink(temp_file);
		return (-1);
	}
	unlink(temp_file);
	return (read_fd);
}

int	handle_heredoc(t_redir *redir, int index)
{
	char	*temp_file;
	int		temp_fd;
	int		read_fd;

	temp_fd = create_temp_file(index, &temp_file);
	if (temp_fd < 0)
		return (-1);
	write_heredoc_input(temp_fd, redir);
	close(temp_fd);
	read_fd = open_and_unlink_heredoc(temp_file);
	if (read_fd < 0)
	{
		free(temp_file);
		return (-1);
	}
	free(temp_file);
	return (read_fd);
}
