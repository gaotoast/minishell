#include "minishell.h"

int	handle_heredoc(t_redir *redir, int index)
{
	char	*index_str;
	char	*temp_file;
	char	*line;
	int		temp_fd;
	int		read_fd;

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
	close(temp_fd);
	read_fd = open(temp_file, O_RDONLY);
	if (read_fd < 0)
	{
		unlink(temp_file);
		free(temp_file);
		return (-1);
	}
	unlink(temp_file);
	free(temp_file);
	return (read_fd);
}
