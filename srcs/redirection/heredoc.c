#include "minishell.h"

// ヒアドキュメントの一時ファイルを生成する
int	create_temp_file(int index, char **temp_file)
{
	char	*index_str;
	size_t	path_len;

	index_str = ft_itoa(index);
	if (!index_str)
		return (1);
	path_len = ft_strlen(HEREDOC_TMP) + ft_strlen(index_str) + 1;
	if (path_len >= PATH_MAX)
	{
		free(index_str);
		return (1);
	}
	*temp_file = ft_strjoin(HEREDOC_TMP, index_str);
	free(index_str);
	if (!(*temp_file))
		return (1);
	return (0);
}

// ヒアドキュメントの入力を一時ファイルに書き込む
int	write_heredoc_input(char *temp_file, t_redir *redir)
{
	int		temp_fd;
	char	*line;

	temp_fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (temp_fd < 0)
	{
		perror("minishell");
		return (-1);
	}
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
        line = get_next_line(STDIN_FILENO);
		if (g_sig_received)
			break;
		if (!line)
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
				redir->str);
			break ;
		}
		if (ft_strncmp(line, redir->str, ft_strlen(redir->str)) == 0
			&& ft_strlen(line) - 1 == ft_strlen(redir->str))
		{
			free(line);
			break ;
		}
		write(temp_fd, line, ft_strlen(line));
		free(line);
	}
	close(temp_fd);
	return (0);
}

// 一時ファイルを開いて、読み込みFDを返してからファイルを削除する
int	open_and_unlink_temp(char *temp_file)
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

// ヒアドキュメントのメイン処理
int	get_heredoc_fd(t_redir *redir, int index)
{
	char	*temp_file;
	int		read_fd;

	if (create_temp_file(index, &temp_file) != 0)
		return (-1);
    set_heredoc_sigint();
	if (write_heredoc_input(temp_file, redir) != 0)
	{
		free(temp_file);
		set_main_sigint();
		return (-1);
	}
	set_main_sigint();
	read_fd = open_and_unlink_temp(temp_file);
	if (read_fd < 0)
	{
		free(temp_file);
		return (-1);
	}
	free(temp_file);
	return (read_fd);
}
