/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 02:47:53 by yumiyao           #+#    #+#             */
/*   Updated: 2025/08/08 21:39:32 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_env(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	while (str[i] && str[i] != '+' && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[0] != '_')
			return (0);
		++i;
	}
	if (str[i] == '+' && str[i + 1] != '=')
		return (0);
	return (1);
}

int	get_eq_idx(char *str, int *plus)
{
	char	*eq;

	*plus = 0;
	if (!str || !is_valid_env(str))
	{
		ft_dprintf(STDERR_FILENO, "minishell: export: `%s"
			"' :not a valid identifier\n", str);
		return (-1);
	}
	eq = ft_strchr(str, '=');
	if (eq && *(eq - 1) == '+')
		*plus = 1;
	return ((int)(eq - str));
}

int	set_env(char *str)
{
	int	i;
	int	plus;

	i = get_eq_idx(str, &plus);
	if (i == -1)
		return (1);
	if (plus)
	{
		if (ft_env(ENV_SET_PLUS, str) == NULL)
		{
			return (2);
		}
		return (0);
	}
	else
	{
		if (ft_env(ENV_SET, str) == NULL)
		{
			return (2);
		}
		return (0);
	}
}

int	export(int argc, char **argv)
{
	int	i;
	int	rtn;
	int	error;

	if (argc == 1)
		return (print_envs());
	i = 1;
	rtn = 0;
	error = 0;
	while (i < argc && argv[i])
	{
		rtn = set_env(argv[i]);
		if (rtn == 2)
		{
			inner_exit(1);
		}
		if (rtn == 1)
			error = 1;
		++i;
	}
	if (error)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
