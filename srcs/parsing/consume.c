/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   consume.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:07:41 by stakada           #+#    #+#             */
/*   Updated: 2025/07/16 13:46:06 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	consume_word(t_token **rest, char **redir_str)
{
	t_token	*cur;

	cur = *rest;
	if (!peek_word(cur))
	{
		if (cur->type == TK_EOF)
			ft_dprintf(STDERR_FILENO,
				"minishell: syntax error near unexpected token `newline'\n");
		else
			ft_dprintf(STDERR_FILENO,
				"minishell: syntax error near unexpected token `%s'\n",
				cur->str);
		(*redir_str) = NULL;
		return (2);
	}
	(*redir_str) = ft_strdup(cur->str);
	if (!(*redir_str))
		return (-1);
	*rest = cur->next;
	return (0);
}

int	consume_reserved(t_token **rest, char *op)
{
	t_token	*cur;

	cur = *rest;
	if (cur->type != TK_RESERVED || ft_strncmp(cur->str, op, ft_strlen(op)
			+ 1) != 0)
		return (0);
	*rest = cur->next;
	return (1);
}
