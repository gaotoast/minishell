/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:27:39 by stakada           #+#    #+#             */
/*   Updated: 2025/08/08 21:27:56 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	split_expanded_redir_env(t_exp_tkn **head, char *str)
{
	int			count;
	t_exp_tkn	*cur;

	if (split_exp_tokens(head) != 0)
	{
		free_exp_tokens(*head);
		return (1);
	}
	count = 0;
	cur = *head;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	if (count > 1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: ambiguous redirect\n", str);
		free_exp_tokens(*head);
		return (1);
	}
	return (0);
}

static int	process_expand_redirs(t_exp_tkn **new, t_redir *redir, int env_flag,
		int last_stat)
{
	if (expand_unsplit(new, redir->str, env_flag, last_stat) != 0)
	{
		free_exp_tokens(*new);
		return (1);
	}
	if (env_flag)
		if (split_expanded_redir_env(new, redir->str) != 0)
			return (1);
	free(redir->str);
	if (!*new)
	{
		redir->str = ft_strdup("");
		if (!redir->str)
			return (1);
	}
	else
	{
		redir->str = (*new)->str;
		free(*new);
	}
	return (0);
}

int	expand_redirs(t_node *node, int last_stat)
{
	int			i;
	t_exp_tkn	*new;
	int			env_flag;

	i = 0;
	while (node->redirs[i])
	{
		new = NULL;
		env_flag = 0;
		if (node->redirs[i]->kind != REDIR_HEREDOC)
			env_flag = 1;
		if (process_expand_redirs(&new, node->redirs[i], env_flag,
				last_stat) != 0)
			return (1);
		i++;
	}
	return (0);
}
