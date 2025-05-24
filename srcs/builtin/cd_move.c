/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_move.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:10:15 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/25 00:59:28 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_longer_split(char **split, char *new, int len)
{
	int		i;
	char	**rtn;

	i = 0;
	rtn = malloc(sizeof(char *) * (len + 2));
	if (!rtn)
		return (NULL);
	while (split[i])
	{
		rtn[i] = split[i];
		++i;
	}
	rtn[i++] = new;
	rtn[i] = NULL;
	free(split);
	return (rtn);
}

char	**make_abs_path(char **dest_split, char **inner_split, int inner_len)
{
	int	i;

	i = 0;
	while (dest_split[i])
	{
		if (ft_strncmp(dest_split[i], "..", 3) == 0 && inner_len > 0)
		{
			--inner_len;
			free(inner_split[inner_len]);
			inner_split[inner_len] = NULL;
		}
		else if (ft_strncmp(dest_split[i], ".", 2) != 0)
		{
			inner_split = get_longer_split(inner_split,
					dest_split[i], inner_len);
			if (!inner_split)
				return (NULL);
			++inner_len;
		}
		++i;
	}
	return (inner_split);
}

char	*get_abs_path(char *dest)
{
	char	*inner_pwd;
	char	**inner_split;
	char	**dest_split;
	char	*rtn;
	int		inner_len;

	//TODO: devとmergeしたときに最初に42_INNER_PWDに値を代入する
	inner_pwd = ft_cwd(PWD_GET, NULL);
	inner_split = ft_split(inner_pwd, '/');
	if (!inner_split)
		return (NULL);
	dest_split = ft_split(dest, '/');
	if (!dest_split)
	{
		free_2d_array(inner_split);
		return (NULL);
	}
	inner_len = 0;
	while (inner_split[inner_len])
		++inner_len;
	inner_split = make_abs_path(dest_split, inner_split, inner_len);
	rtn = ft_union(inner_split, '/');
	free_2d_array(inner_split);
	free(dest_split);
	return (rtn);
}

char	*move_to_some(char *dest)
{
	char	*path;
	int		res;

	if (dest[0] == '/')
		path = dest;
	else if (ft_strncmp("-", dest, 2) == 0)
		path = move_to_env("OLDPWD");
	else if (ft_strncmp("./", dest, 2) == 0)
		path = get_abs_path(dest);
	else
		path = get_abs_path(ft_strjoin("./", dest));
	res = chdir(path);
	if (res != 0)
	{
		perror("minishell: ");
		if (dest[0] != '/' && ft_strncmp("-", dest, 2) != 0)
			free(path);
		return (NULL);
	}
	return (path);
}
