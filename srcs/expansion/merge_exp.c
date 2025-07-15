/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_exp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:25:32 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 14:25:34 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	should_merge_tokens(t_exp_tkn *cur, t_exp_tkn *next)
{
	if (cur->is_quoted == next->is_quoted && cur->is_expanded == true
		&& next->is_expanded == true)
		return (1);
	return (0);
}

int	merge_expansion_tokens(t_exp_tkn **head)
{
	t_exp_tkn	*cur;
	t_exp_tkn	*next;
	char		*merged_str;

	if (!head)
		return (0);
	cur = *head;
	while (cur && cur->next)
	{
		next = cur->next;
		if (should_merge_tokens(cur, next))
		{
			merged_str = ft_strjoin(cur->str, next->str);
			if (!merged_str)
				return (1);
			free(cur->str);
			free(next->str);
			cur->str = merged_str;
			cur->next = next->next;
			free(next);
		}
		else
			cur = cur->next;
	}
	return (0);
}
