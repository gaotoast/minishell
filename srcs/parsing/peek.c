/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   peek.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:08:01 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 15:08:03 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	peek_word(t_token *token)
{
	if (token->type == TK_WORD)
		return (1);
	return (0);
}

int	peek_redir_op(t_token *token)
{
	if (token->type != TK_RESERVED)
		return (0);
	if (ft_strncmp(token->str, ">>", 3) == 0 || ft_strncmp(token->str, ">",
			2) == 0 || ft_strncmp(token->str, "<<", 3) == 0
		|| ft_strncmp(token->str, "<", 2) == 0)
		return (1);
	return (0);
}
