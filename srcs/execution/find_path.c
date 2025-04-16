#include "minishell.h"

// 環境変数PATHの値を一つずつ確認
char	*search_path(char *cmd_name, char **path_list)
{
	char	*path;
	char	*path_tail;
	int		i;

	i = 0;
	while (path_list[i])
	{
		path_tail = ft_strjoin("/", cmd_name);
		path = ft_strjoin(path_list[i], path_tail);
		free(path_tail);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

// 環境変数PATHからコマンドのパスを探す
char	*resolve_cmd_path(char *cmd, char *path_env)
{
	char	**path_list;
	char	*filepath;

	path_list = ft_split(path_env, ':');
	if (!path_list)
	{
		perror("minishell");
		return (NULL);
	}
	filepath = search_path(cmd, path_list);
	if (!filepath)
	{
		ft_dprintf(STDERR_FILENO, "%s: command not found\n", cmd);
		free_2d_array((void **)path_list);
		return (NULL);
	}
	return (filepath);
}
