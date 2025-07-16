/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   append_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 17:26:37 by stakada           #+#    #+#             */
/*   Updated: 2025/07/16 13:47:42 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_redir	**get_new_redirs(t_node *node, t_token **cur, int *ret)
{
	t_redir	**redirs;
	int		i;

	redirs = (t_redir **)ft_malloc(sizeof(t_redir *) * (node->redir_count + 1));
	if (!redirs)
	{
		*ret = -1;
		return (NULL);
	}
	i = 0;
	while (i < node->redir_count - 1)
	{
		redirs[i] = node->redirs[i];
		i++;
	}
	redirs[node->redir_count - 1] = parse_redir(cur, ret);
	if (!redirs[node->redir_count - 1])
	{
		free(redirs);
		return (NULL);
	}
	redirs[node->redir_count] = NULL;
	return (redirs);
}

int	append_redir(t_node *node, t_token **cur, int *ret)
{
	t_redir	**redirs_tmp;

	node->redir_count++;
	redirs_tmp = get_new_redirs(node, cur, ret);
	if (!redirs_tmp)
		return (-1);
	free(node->redirs);
	node->redirs = redirs_tmp;
	return (0);
}
