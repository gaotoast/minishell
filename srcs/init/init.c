/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:28:11 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 14:32:46 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_signals(void)
{
	set_main_sigint();
	set_main_sigquit();
}

int	init_env(char **envp)
{
	t_env	*head;
	int		i;

	i = 0;
	while (envp[i])
	{
		head = (t_env *)ft_env(ENV_ADD, envp[i]);
		if (!head)
		{
			ft_env(ENV_DEL_ALL, NULL);
			return (1);
		}
		++i;
	}
	return (0);
}

int	init_pwd(void)
{
	char	*pwd;

	pwd = ft_env(ENV_GET_VAL, "PWD");
	if (!pwd)
	{
		pwd = (char *)ft_malloc(sizeof(char) * (PATH_MAX + 1));
		if (!pwd)
			return (1);
		getcwd(pwd, PATH_MAX);
	}
	else
	{
		pwd = ft_strdup(pwd);
		if (!pwd)
		{
			return (1);
		}
	}
	ft_cwd(PWD_SET, pwd);
	return (0);
}

int	init_shlvl(void)
{
	char	*shlvl;
	int		lvl;

	shlvl = ft_env(ENV_GET_VAL, "SHLVL");
	if (!shlvl || is_valid_shlvl(shlvl))
	{
		if (ft_env(ENV_ADD, "SHLVL=1") == NULL)
			return (1);
		return (0);
	}
	lvl = pure_atoi(shlvl);
	return (handle_shlvl_range(lvl));
}

int	init(char **envp)
{
	t_shell	*shell;

	shell = (t_shell *)ft_malloc(sizeof(t_shell));
	if (!shell)
		return (-1);
	sh_op(SH_SET, shell);
	sh_stat(ST_SET, 0);
	shell->tokens = NULL;
	if (init_env(envp))
	{
		free(shell);
		return (-1);
	}
	if (init_pwd())
	{
		free(shell);
		ft_env(ENV_DEL_ALL, NULL);
		return (-1);
	}
	if (init_shlvl())
		inner_exit(1);
	shell->ast = NULL;
	return (0);
}
