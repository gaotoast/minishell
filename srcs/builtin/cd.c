/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 03:40:56 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/25 09:13:27 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*move_to_env(char *val_name)
{
	int		res;
	t_env	*env;

	env = (t_env *)ft_env(ENV_GET_STRUCT, val_name);
	if (!env)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s not set\n", val_name);
		return (NULL);
	}
	res = chdir(env->val);
	if (res != 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: cd: %s", strerror(errno));
		return (NULL);
	}
	return (env->val);
}

void	update_envs(char *path)
{
	t_env	*old_pwd;
	t_env	*pwd;
	char	*cp_path;

	cp_path = ft_strdup(path);
	if (!cp_path)
		return ;
	old_pwd = (t_env *)ft_env(ENV_GET_STRUCT, "OLDPWD");
	pwd = (t_env *)ft_env(ENV_GET_STRUCT, "PWD");
	if (pwd && old_pwd)
		ft_env(ENV_SET, ft_strjoin("OLDPWD=", pwd->val));
	if (pwd)
		ft_env(ENV_SET, ft_strjoin("PWD=", path));
	ft_cwd(PWD_SET, cp_path);
}

int	cd(int argc, char **argv)
{
	char	*path;

	if (argc > 2)
	{
		write(STDERR_FILENO, "minishell: cd: too many arguments\n", 34);
		return (EXIT_FAILURE);
	}
	if (argc == 1)
		path = move_to_env("HOME");
	else
		path = move_to_some(argv[1]);
	if (!path)
		return (EXIT_FAILURE);
	else
	{
		update_envs(path);
	}
	return (EXIT_SUCCESS);
}
