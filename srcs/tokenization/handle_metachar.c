/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_metachar.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:26:21 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 16:26:54 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*handle_two_metachar(t_token *cur, char **p, int *ret)
{
	cur = add_token(cur, TK_RESERVED, *p, 2);
	if (!cur)
	{
		*ret = -1;
		return (NULL);
	}
	*p += 2;
	return (cur);
}

t_token	*handle_single_metachar(t_token *cur, char **p, int *ret)
{
	cur = add_token(cur, TK_RESERVED, *p, 1);
	if (!cur)
	{
		*ret = -1;
		return (NULL);
	}
	(*p)++;
	return (cur);
}
