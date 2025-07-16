/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:32:55 by stakada           #+#    #+#             */
/*   Updated: 2025/07/16 14:06:51 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_node	*check_syntax(t_node *node, int *ret)
{
	if (*ret == 0 && node->argc == 0 && node->redir_count == 0)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		free_ast(node);
		*ret = 2;
		return (NULL);
	}
	return (node);
}

static int	process_tokens(t_node *node, t_token **cur, int *ret)
{
	while (*cur)
	{
		if (peek_word(*cur) && append_arg(node, (*cur)->str, ret) == 0)
			*cur = (*cur)->next;
		else if (peek_redir_op(*cur) && append_redir(node, cur, ret) == 0)
			continue ;
		else
			break ;
		if (*ret != 0)
			return (-1);
	}
	return (0);
}

t_node	*parse_command(t_token **rest, int *ret)
{
	t_token	*cur;
	t_node	*node;

	cur = *rest;
	node = new_command_node();
	if (!node)
	{
		*ret = -1;
		return (NULL);
	}
	if (process_tokens(node, &cur, ret) != 0)
	{
		free_ast(node);
		return (NULL);
	}
	*rest = cur;
	return (check_syntax(node, ret));
}
