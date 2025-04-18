/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 02:47:53 by yumiyao           #+#    #+#             */
/*   Updated: 2025/04/18 04:22:42 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "minishell.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <limits.h>
# include <linux/limits.h>
# include <string.h>

void	print_env(char *env)
{
	int	i;

	write(STDOUT_FILENO, "declare -x ", 11);
	i = 0;
	while (env[i] && env[i] != '=')
		write(STDOUT_FILENO, &env[i++], 1);
	++i;
	if (env[i])
	{
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, env + i, strlen(env + i));
		write(STDOUT_FILENO, "\"", 1);
	}
	write(STDOUT_FILENO, "\n", 1);
}

int	print_envs(char **envp, int len)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	while (i < len)
	{
		j = i + 1;
		while (j < len - 1)
		{
			if (strcmp(envp[i], envp[j]) > 0)
			{
				tmp = envp[i];
				envp[i] = envp[j];
				envp[j] = tmp;
			}
			++j;
		}
		++i;
	}
	while (*envp)
		print_env(*(envp++));
	return (EXIT_SUCCESS);
}

void	set_input(char **argv, char **envp)
{
	return ;
}

int	export(int argc, char **argv, char **envp)
{
	int	len;

	len = 0;
	while (envp[len])
		++len;
	if (argc == 1)
		return (print_envs(envp, len));
	set_input(argv, envp);
}

int main(int argc, char **argv, char **envp)
{
	export(argc, argv, envp);
}
