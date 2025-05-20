/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 03:40:56 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/21 06:00:29 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_name(char *name)
{
	int	i;

	i = 0;
	if (ft_isdigit(name[0]))
		return (0);
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		++i;
	}
	return (1);
}

int	ft_getenvidx(char *name, char ***envp)
{
	int	i;
	int	name_len;

	if (!is_valid_name(name))
		return (-1);
	i = 0;
	name_len = ft_strlen(name);
	while ((*envp)[i])
	{
		if (ft_strncmp(name, (*envp)[i], name_len) == 0
			&& (*envp)[i][name_len] == '=')
			return (i);
		++i;
	}
	return (-1);
}

char	*ft_getenv(char *name, char **envp)
{
	size_t	name_len;
	int		i;

	if (!envp)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if ((ft_strncmp(name, envp[i], name_len) == 0)
			&& envp[i][name_len] == '=')
			return (&envp[i][name_len + 1]);
		i++;
	}
	return (NULL);
}

char	*ft_union(char **split, char delim)
{
	int		i;
	int		tmp_len;
	int		len;
	char	*rtn;

	i = 0;
	len = 0;
	if (!split)
		return (NULL);
	while (split[i])
		len += ft_strlen(split[i++]);
	rtn = malloc(sizeof(char) * (len + i + 1));
	if (!rtn)
		return (NULL);
	i = 0;
	rtn[len] = '\0';
	len = 0;
	while (split[i])
	{
		rtn[len++] = delim;
		tmp_len = strlen(split[i]);
		ft_strlcpy(&(rtn[len]), split[i++], tmp_len + 1);
		len += tmp_len;
	}
	return (rtn);
}

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
	inner_pwd = ft_getcwd(PWD_GET, NULL);
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
	free_2d_array(dest_split);
	return (rtn);
}

char	*move_to_env(char ***envp, char *val_name)
{
	int		val_idx;
	int		res;
	char	*path;

	val_idx = ft_getenvidx(val_name, envp);
	if (val_idx < 0)
	{
		write(STDERR_FILENO, "minishell: cd: ", 16);
		write(STDERR_FILENO, val_name, ft_strlen(val_name));
		write(STDERR_FILENO, " not set\n", 9);
		return (NULL);
	}
	res = chdir((*envp)[val_idx] + ft_strlen(val_name) + 1);
	path = ft_getenv(val_name, *envp);
	if (res != 0)
	{
		perror("minishell: ");
		return (NULL);
	}
	return (path);
}

char	*move_to_some(char *dest, char ***envp)
{
	char	*path;
	int		res;

	if (dest[0] == '/')
		path = dest;
	else if (ft_strncmp("-", dest, 2) == 0)
		path = move_to_env(envp, "OLDPWD");
	else if (ft_strncmp("./", dest, 2) == 0)
		path = get_abs_path(dest);
	else
		path = get_abs_path(ft_strjoin("./", dest));
	res = chdir(path);
	if (res != 0)
	{
		perror("minishell: ");
		if (dest[0] != '/')
			free(path);
		return (NULL);
	}
	return (path);
}

void	update_envs(char *path, char ***envp)
{
	char	*old_pwd;
	char	*pwd;
	int		i;

	i = 0;
	while ((*envp)[i])
		++i;
	old_pwd = ft_getenv("OLDPWD", *envp);
	pwd = ft_getenv("PWD", *envp);
	if (!pwd && old_pwd)
		set_env(ft_strdup("OLDPWD="), envp, &i);
	if (pwd)
		set_env(ft_strjoin("PWD=", path), envp, &i);
	ft_getcwd(PWD_SET, path);
}

void	free_2d_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	cd(int argc, char **argv, char ***envp)
{
	char	*path;

	if (argc > 2)
	{
		write(STDERR_FILENO, "minishell: cd: too many arguments\n", 34);
		return (EXIT_FAILURE);
	}
	if (argc == 1)
		path = move_to_env(envp, "HOME");
	else
		path = move_to_some(argv[1], envp);
	if (!path)
		return (EXIT_FAILURE);
	else
	{
		update_envs(path, envp);
	}
	return (EXIT_SUCCESS);
}
