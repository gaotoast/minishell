/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_node.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:08:15 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 15:08:38 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*new_pipe_node(t_node *lhs, t_node *rhs)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
	{
		free_ast(lhs);
		free_ast(rhs);
		return (NULL);
	}
	node->kind = ND_PIPE;
	node->argv = NULL;
	node->argv_lst = NULL;
	node->argc = 0;
	node->redirs = NULL;
	node->redir_count = 0;
	node->lhs = lhs;
	node->rhs = rhs;
	node->next_cmd = NULL;
	node->in_fd = -1;
	node->pipefd[0] = -1;
	node->pipefd[1] = -1;
	return (node);
}

t_node	*new_command_node(void)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->kind = ND_CMD;
	node->argv = NULL;
	node->argv_lst = NULL;
	node->argc = 0;
	node->redirs = NULL;
	node->redir_count = 0;
	node->lhs = NULL;
	node->rhs = NULL;
	node->next_cmd = NULL;
	node->in_fd = STDIN_FILENO;
	node->pipefd[0] = -1;
	node->pipefd[1] = -1;
	return (node);
}
