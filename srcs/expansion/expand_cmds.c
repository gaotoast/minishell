/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmds.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:25:22 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 14:25:24 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_expand_cmds(t_exp_tkn **head, char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (expand_unsplit(head, argv[i], 1) != 0)
		{
			free_exp_tokens(*head);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	expand_cmds(t_node *node)
{
	t_exp_tkn	*head;

	head = NULL;
	if (process_expand_cmds(&head, node->argv) != 0)
		return (-1);
	if (split_exp_tokens(&head) != 0)
	{
		free_exp_tokens(head);
		return (-1);
	}
	if (update_args_from_exp(head, node) != 0)
		return (-1);
	return (0);
}
