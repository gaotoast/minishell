/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:25:00 by stakada           #+#    #+#             */
/*   Updated: 2025/08/08 21:41:11 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*search_path(char *cmd_name, char **path_list, char *path_tail,
		int *status)
{
	char	*path;
	int		i;

	i = 0;
	while (path_list[i])
	{
		path = ft_strjoin(path_list[i], path_tail);
		if (!path)
			return (NULL);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	ft_dprintf(STDERR_FILENO, "%s: command not found\n", cmd_name);
	(*status) = 127;
	return (NULL);
}

char	*resolve_cmd_path(char *cmd, char *path_env, int *status)
{
	char	*path_tail;
	char	**path_list;
	char	*filepath;

	path_tail = ft_strjoin("/", cmd);
	if (!path_tail)
		return (NULL);
	path_list = ft_split(path_env, ':');
	if (!path_list)
	{
		free(path_tail);
		return (NULL);
	}
	filepath = search_path(cmd, path_list, path_tail, status);
	if (!filepath)
	{
		free(path_tail);
		free_2d_array(path_list);
		return (NULL);
	}
	return (filepath);
}
