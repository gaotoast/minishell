/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:06:48 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 16:33:47 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse_line(t_token **rest, int *ret)
{
	t_node	*node;
	t_node	*rhs;

	node = parse_command(rest, ret);
	if (!node)
		return (NULL);
	while (consume_reserved(rest, "|"))
	{
		rhs = parse_command(rest, ret);
		if (!rhs)
		{
			free_ast(node);
			return (NULL);
		}
		node = new_pipe_node(node, rhs);
		if (!node)
		{
			*ret = -1;
			return (NULL);
		}
	}
	return (node);
}

int	parse(t_token *tokens, t_node **ast)
{
	t_token	*rest;
	int		ret;

	ret = 0;
	if (tokens->type == TK_EOF)
	{
		(*ast) = NULL;
		return (0);
	}
	rest = tokens;
	(*ast) = parse_line(&rest, &ret);
	return (ret);
}
