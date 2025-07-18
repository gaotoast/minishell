/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:38:39 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 14:38:51 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*add_token(t_token *cur, t_token_type type, char *start, int len)
{
	t_token	*new;

	new = (t_token *)ft_malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->str = (char *)ft_calloc(sizeof(char), len + 1);
	if (!new->str)
	{
		free(new);
		return (NULL);
	}
	ft_memcpy(new->str, (void *)start, len);
	new->type = type;
	new->next = NULL;
	cur->next = new;
	return (new);
}
