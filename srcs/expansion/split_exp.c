/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:26:53 by stakada           #+#    #+#             */
/*   Updated: 2025/08/08 21:45:13 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_exp_token_from_substr(t_exp_tkn **head, char **p, int len,
		bool is_quoted)
{
	char		*str;
	t_exp_tkn	*new;

	if (len > 0)
		str = ft_strndup(*p, len);
	else
		str = ft_strdup("");
	if (!str)
		return (-1);
	*p += len;
	new = new_exp_token(str, true, is_quoted);
	if (!new)
	{
		free(str);
		return (-1);
	}
	append_exp_token(head, new);
	return (0);
}

static t_exp_tkn	*split_by_blank(char *s, bool is_quoted)
{
	t_exp_tkn	*head;
	char		*p;
	int			start;
	int			len;

	head = NULL;
	p = s;
	while (*p)
	{
		while (*p && is_blank(*p))
			p++;
		start = 0;
		while (p[start] && !is_blank(p[start]))
			start++;
		len = start;
		if (add_exp_token_from_substr(&head, &p, len, is_quoted) != 0)
		{
			free_exp_tokens(head);
			return (NULL);
		}
	}
	return (head);
}

static int	process_split_exp_tokens(t_exp_tkn **head, t_exp_tkn *cur,
		t_exp_tkn **prev)
{
	t_exp_tkn	*split_head;
	t_exp_tkn	*split_tail;

	split_head = split_by_blank(cur->str, cur->is_quoted);
	if (!split_head)
		return (-1);
	split_tail = split_head;
	while (split_tail->next)
		split_tail = split_tail->next;
	if (*prev)
		(*prev)->next = split_head;
	else
		*head = split_head;
	split_tail->next = cur->next;
	free(cur->str);
	free(cur);
	*prev = split_tail;
	return (0);
}

int	split_exp_tokens(t_exp_tkn **head)
{
	t_exp_tkn	*cur;
	t_exp_tkn	*prev;

	cur = *head;
	prev = NULL;
	while (cur)
	{
		if (cur->is_expanded && !cur->is_quoted && (ft_strchr(cur->str, ' ')
				|| ft_strchr(cur->str, '\t')))
		{
			if (process_split_exp_tokens(head, cur, &prev) != 0)
				return (-1);
			cur = prev->next;
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
	return (0);
}
