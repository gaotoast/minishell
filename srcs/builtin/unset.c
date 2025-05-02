/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 21:58:27 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/02 22:17:59 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_getenv(char *name, char ***envp)
{
	int	i;
	int	name_len;

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

int	rm_env(int rm_idx, char ***envp)
{
	int		env_len;
	char	**new;
	int		i;

	env_len = 0;
	while ((*envp)[env_len])
		++env_len;
	new = (char **)malloc(sizeof(char *) * (env_len));
	if (!new)
	{
		perror("minishell");
		return (EXIT_FAILURE);
	}
	i = 0;
	while (env_len >= 0)
	{
		if (env_len != rm_idx)
			new[i++] = (*envp)[env_len];
		else
			free((*envp)[env_len]);
		--env_len;
	}
	free(*envp);
	*envp = new;
	return (EXIT_SUCCESS);
}

int	unset(int argc, char **argv, char ***envp)
{
	int	env_idx;

	env_idx = ft_getenv(argv[1], envp);
	if (env_idx < 0)
		return (EXIT_SUCCESS);
	return (rm_env(env_idx, envp));
}
