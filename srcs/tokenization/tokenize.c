/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:01:54 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 16:28:02 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*get_next_token(t_token *cur, char **p, int *ret)
{
	if (is_blank(**p))
		(*p)++;
	else if (is_two_metachar(*p))
		cur = handle_two_metachar(cur, p, ret);
	else if (is_single_metachar(*p))
		cur = handle_single_metachar(cur, p, ret);
	else
		cur = handle_word_token(cur, p, ret);
	return (cur);
}

int	tokenize(char *line, t_token **tokens)
{
	t_token	head;
	t_token	*cur;
	char	*p;
	int		ret;

	head.next = NULL;
	cur = &head;
	p = line;
	ret = 0;
	while (*p)
	{
		cur = get_next_token(cur, &p, &ret);
		if (ret != 0)
		{
			free_tokens(head.next);
			return (ret);
		}
	}
	if (!add_token(cur, TK_EOF, p, 0))
	{
		free_tokens(head.next);
		return (-1);
	}
	(*tokens) = head.next;
	return (0);
}
