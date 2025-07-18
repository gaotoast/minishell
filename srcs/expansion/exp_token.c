/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:27:59 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 14:28:05 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_exp_tkn	*new_exp_token(char *str, bool is_expanded, bool is_quoted)
{
	t_exp_tkn	*new;

	new = (t_exp_tkn *)ft_malloc(sizeof(t_exp_tkn));
	if (!new)
	{
		free(str);
		return (NULL);
	}
	new->str = str;
	new->is_expanded = is_expanded;
	new->is_quoted = is_quoted;
	new->next = NULL;
	return (new);
}

void	append_exp_token(t_exp_tkn **head, t_exp_tkn *new)
{
	t_exp_tkn	*cur;

	if (!*head)
	{
		*head = new;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

void	free_exp_tokens(t_exp_tkn *head)
{
	t_exp_tkn	*cur;
	t_exp_tkn	*next;

	if (!head)
		return ;
	cur = head;
	while (cur)
	{
		next = cur->next;
		free(cur->str);
		free(cur);
		cur = next;
	}
}
