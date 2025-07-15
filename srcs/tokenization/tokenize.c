/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:01:54 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 15:02:47 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: 分割
int	tokenize(char *line, t_token **tokens)
{
	t_token	head;
	t_token	*cur;
	char	*p;
	char	*start;
	char	quote;

	head.next = NULL;
	cur = &head;
	p = line;
	while (*p)
	{
		if (is_blank(*p))
			p++;
		else if (is_two_metachar(p))
		{
			cur = add_token(cur, TK_RESERVED, p, 2);
			if (!cur)
			{
				free_tokens(head.next);
				return (-1);
			}
			p += 2;
		}
		else if (is_single_metachar(p))
		{
			cur = add_token(cur, TK_RESERVED, p, 1);
			if (!cur)
			{
				free_tokens(head.next);
				return (-1);
			}
			p++;
		}
		else
		{
			start = p;
			while (*p && !is_blank(*p) && !is_two_metachar(p)
				&& !is_single_metachar(p))
			{
				if (is_quote(p))
				{
					quote = *p;
					p++;
					while (*p && *p != quote)
						p++;
					if (*p == quote)
						p++;
					else
					{
						write(STDERR_FILENO,
							"minishell: syntax error: unclosed quote\n", 41);
						free_tokens(head.next);
						return (2);
					}
				}
				else
					p++;
			}
			cur = add_token(cur, TK_WORD, start, p - start);
			if (!cur)
			{
				free_tokens(head.next);
				return (-1);
			}
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
