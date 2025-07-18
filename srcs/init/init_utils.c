/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:29:44 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 14:35:29 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_envp_copy(char **envp)
{
	char	**cp;
	int		env_count;
	int		i;

	env_count = 0;
	while (envp[env_count])
		env_count++;
	cp = (char **)ft_malloc(sizeof(char *) * (env_count + 1));
	if (!cp)
		return (NULL);
	i = 0;
	while (i < env_count)
	{
		cp[i] = ft_strdup(envp[i]);
		if (!cp[i])
		{
			while (--i >= 0)
				free(cp[i]);
			free(cp);
			return (NULL);
		}
		i++;
	}
	cp[i] = NULL;
	return (cp);
}

int	pure_atoi(char *num)
{
	int	rtn;
	int	i;
	int	sign;

	rtn = 0;
	i = 0;
	sign = 1;
	if (ft_isspace(num[i]))
		++i;
	if (num[i] == '-')
	{
		sign = -1;
		++i;
	}
	else if (num[i] == '+')
		++i;
	while (ft_isdigit(num[i]))
	{
		rtn *= 10;
		rtn += num[i] - '0';
		++i;
	}
	return (rtn * sign);
}

int	is_valid_shlvl(char *num)
{
	int	i;

	i = 0;
	while (ft_isspace(num[i]))
		++i;
	if (num[i] == '+' || num[i] == '-')
		++i;
	while (ft_isdigit(num[i]))
		++i;
	while (ft_isspace(num[i]))
		++i;
	if (num[i] == '\0')
		return (0);
	return (1);
}

static int	increment_shlvl(int shlvl)
{
	char	*str_shlvl;
	char	*env_shlvl;

	str_shlvl = ft_itoa(shlvl + 1);
	env_shlvl = ft_strjoin("SHLVL=", str_shlvl);
	if (!str_shlvl || !env_shlvl)
	{
		free(str_shlvl);
		free(env_shlvl);
		ft_env(ENV_DEL_ALL, NULL);
		return (1);
	}
	if (ft_env(ENV_SET, env_shlvl) == NULL)
	{
		free(str_shlvl);
		free(env_shlvl);
		ft_env(ENV_DEL_ALL, NULL);
		return (1);
	}
	free(str_shlvl);
	free(env_shlvl);
	return (0);
}

int	handle_shlvl_range(int lvl)
{
	if (lvl > 998)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: warning: shell level (%d) too high, resetting to 1\n",
			lvl + 1);
		if (ft_env(ENV_SET, "SHLVL=1") == NULL)
			return (1);
	}
	else if (lvl < 0)
	{
		if (ft_env(ENV_SET, "SHLVL=0") == NULL)
			return (1);
	}
	else if (increment_shlvl(lvl))
		return (1);
	return (0);
}
