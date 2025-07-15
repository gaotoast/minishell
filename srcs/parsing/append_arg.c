/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_arg.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:25:56 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 17:26:19 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_new_argv(t_node *node, char *str, int *ret)
{
	char	**argv;
	int		i;

	argv = (char **)ft_malloc(sizeof(char *) * (node->argc + 1));
	if (!argv)
	{
		*ret = -1;
		return (NULL);
	}
	i = 0;
	while (i < node->argc - 1)
	{
		argv[i] = node->argv[i];
		i++;
	}
	argv[node->argc - 1] = ft_strdup(str);
	if (!argv[node->argc - 1])
	{
		free(argv);
		*ret = -1;
		return (NULL);
	}
	argv[node->argc] = NULL;
	return (argv);
}

int	append_arg(t_node *node, char *str, int *ret)
{
	char	**argv_tmp;

	node->argc++;
	argv_tmp = get_new_argv(node, str, ret);
	if (!argv_tmp)
		return (-1);
	free(node->argv);
	node->argv = argv_tmp;
	return (0);
}
