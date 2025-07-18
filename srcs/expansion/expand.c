/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:26:02 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 14:26:03 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand(t_node *node)
{
	if (!node)
		return (0);
	if (node->kind == ND_CMD)
	{
		if (node->argv)
		{
			if (expand_cmds(node) != 0)
				return (1);
		}
		if (node->redirs)
		{
			if (expand_redirs(node) != 0)
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
