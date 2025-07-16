/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   link_exec_nodes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:15:01 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 14:15:17 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	link_exec_nodes(t_node *node, t_node *rhs, t_node **first,
		t_node **last)
{
	if (node->kind == ND_CMD)
	{
		*first = node;
		return ;
	}
	if (node->lhs->kind == ND_CMD)
	{
		node->lhs->next_cmd = rhs;
		*first = node->lhs;
		*last = rhs;
		return ;
	}
	if (node->lhs && node->lhs->rhs)
		link_exec_nodes(node->lhs, node->lhs->rhs, first, last);
	if (*last)
	{
		(*last)->next_cmd = rhs;
		*last = rhs;
	}
}
