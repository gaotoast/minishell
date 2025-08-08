/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:26:02 by stakada           #+#    #+#             */
/*   Updated: 2025/08/08 21:28:12 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand(t_node *node)
{
	int	last_stat;

	last_stat = sh_stat(ST_GET, 0);
	if (!node)
		return (0);
	if (node->kind == ND_CMD)
	{
		if (node->argv)
		{
			if (expand_cmds(node, last_stat) != 0)
				return (1);
		}
		if (node->redirs)
		{
			if (expand_redirs(node, last_stat) != 0)
				return (1);
		}
		return (0);
	}
	if (expand(node->lhs) != 0)
		return (1);
	if (expand(node->rhs) != 0)
		return (1);
	return (0);
}
