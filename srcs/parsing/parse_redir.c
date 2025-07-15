/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 16:33:37 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 17:06:54 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	set_redir_kind(t_token **rest, t_redir *redir, int *ret)
{
	if (consume_reserved(rest, ">>"))
		redir->kind = REDIR_APPEND;
	else if (consume_reserved(rest, ">"))
		redir->kind = REDIR_OUT;
	else if (consume_reserved(rest, "<<"))
		redir->kind = REDIR_HEREDOC;
	else if (consume_reserved(rest, "<"))
		redir->kind = REDIR_IN;
	else
	{
		*ret = -1;
		return (-1);
	}
	return (0);
}

t_redir	*parse_redir(t_token **rest, int *ret)
{
	t_redir	*redir;

	redir = ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->temp_file = NULL;
	if (set_redir_kind(rest, redir, ret) != 0)
	{
		free(redir);
		return (NULL);
	}
	*ret = consume_word(rest, &redir->str);
	if (*ret != 0)
	{
		free(redir);
		return (NULL);
	}
	return (redir);
}
