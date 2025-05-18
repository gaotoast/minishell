/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 03:40:56 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/18 21:48:11 by yumiyao          ###   ########.fr       */
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
	while (split[i])
	{
		len += ft_strlen(split[i]);
		++i;
	}
	rtn = malloc(sizeof(char) * (len + i + 1));
	if (!rtn)
		return (NULL);
	i = 0;
	rtn[len] = '\0';
	len = 0;
	while (split[i])
	{
		tmp_len = strlen(split[i]);
		rtn[len++] = delim;
		ft_strlcpy(&(rtn[len]), split[i], tmp_len);
		len += tmp_len;
		++i;
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

char	*get_abs_path(char *dest, char ***envp)
{
	char	*inner_pwd;
	char	**inner_split;
	char	**dest_split;
	int		inner_len;
	int		i;

	inner_pwd = ft_getenv("TEST_INNER_PWD", *envp);
	inner_split = ft_split(inner_pwd, '/');
	dest_split = ft_split(dest, '/');
	i = 0;
	inner_len = 0;
	while (inner_split[inner_len])
		++inner_len;
	while (dest_split[i])
	{
		if (ft_strncmp(dest_split[i], "..", 3) == 0 && inner_len > 0)
		{
			--inner_len;
			free(inner_split[inner_len]);
			inner_split[inner_len] = NULL;
		}
		else if (ft_strncmp(dest_split[i], ".", 2) == 0)
		{
			inner_split = get_longer_split(inner_split,
					dest_split[i], inner_len);
			if (!inner_split)
				return (NULL);
			++inner_len;
		}
		++i;
	}
	for(int k = 0; dest_split[k]; ++k)
	{
		printf("in_sp: %s\n", dest_split[k]);
	}
	char *rtn = ft_union(inner_split, '/');
	printf("rtn:%s\n", rtn);
	return (rtn);
}

void	build_full_path(char *dest, char ***envp, int i)
{
	if (ft_strncmp("./", dest, 2) == 0)
		set_env(ft_strjoin("PWD=", get_abs_path(dest, envp)), envp, &i);
	else
		set_env(ft_strjoin("PWD=", get_abs_path(ft_strjoin("./", dest), envp)), envp, &i);
}

void	update_pwd(char *dest, char ***envp)
{
	int	i;

	i = 0;
	while ((*envp)[i])
		++i;
	if (dest[0] == '/')
		set_env(ft_strjoin("PWD=", dest), envp, &i);
	else
		build_full_path(dest, envp, i);
}

int	move_to_home(char ***envp)
{
	int	home_idx;

	home_idx = ft_getenvidx("HOME", envp);
	if (home_idx < 0)
	{
		write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
		return (EXIT_FAILURE);
	}
	update_pwd(ft_getenv("HOME", *envp), envp);
	return (chdir((*envp)[home_idx] + 5));
}

int	move_to_some(char *dest, char ***envp)
{
	update_pwd(dest, envp);
	return (chdir(dest));
}


int	cd(int argc, char **argv, char ***envp)
{
	int		res;

	if (argc > 2)
	{
		write(STDERR_FILENO, "minishell: cd: too many arguments\n", 34);
		return (EXIT_FAILURE);
	}
	if (argc == 1)
	{
		res = move_to_home(envp);
	}
	else
		res = move_to_some(argv[1], envp);
	if (res != EXIT_SUCCESS)
	{
		perror("minishell");
		return (EXIT_FAILURE);
	}
	printf("pwd: %s\n", getcwd(NULL, PATH_MAX));
	printf("$PWD: %s\n", ft_getenv("PWD", *envp));
	return (EXIT_SUCCESS);
}
