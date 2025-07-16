/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:27:51 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 16:28:14 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_quoted_string(char **p)
{
	char	quote;

	quote = **p;
	(*p)++;
	while (**p && **p != quote)
		(*p)++;
	if (**p == quote)
		(*p)++;
	else
	{
		write(STDERR_FILENO, "minishell: syntax error: unclosed quote\n", 41);
		return (2);
	}
	return (0);
}

t_token	*handle_word_token(t_token *cur, char **p, int *ret)
{
	char	*start;

	start = *p;
	while (**p && !is_blank(**p) && !is_two_metachar(*p)
		&& !is_single_metachar(*p))
	{
		if (is_quote(*p))
		{
			*ret = skip_quoted_string(p);
			if (*ret != 0)
				return (NULL);
		}
		else
			(*p)++;
	}
	cur = add_token(cur, TK_WORD, start, *p - start);
	return (cur);
}
