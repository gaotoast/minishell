/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 03:40:56 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/22 16:31:31 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_getenv_idx(char *name, char ***envp)
{
	int	i;
	int	name_len;

	if (!is_valid_env(name))
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

char	*move_to_env(char ***envp, char *val_name)
{
	int		val_idx;
	int		res;
	char	*path;

	val_idx = ft_getenv_idx(val_name, envp);
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

void	update_envs(char *path, char ***envp)
{
	char	*old_pwd;
	char	*pwd;
	int		i;
	char	*cp_path;

	i = 0;
	cp_path = ft_strdup(path);
	if (!cp_path)
		return ;
	while ((*envp)[i])
		++i;
	old_pwd = ft_getenv("OLDPWD", *envp);
	pwd = ft_getenv("PWD", *envp);
	if (pwd && old_pwd)
		set_env(ft_strjoin("OLDPWD=", pwd), envp, &i);
	if (pwd)
		set_env(ft_strjoin("PWD=", path), envp, &i);
	ft_cwd(PWD_SET, cp_path);
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
