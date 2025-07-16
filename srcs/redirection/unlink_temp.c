/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unlink_temp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:08:28 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 15:08:29 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unlink_all_temp(t_node *first)
{
	t_node	*cur;
	int		i;

	if (!first)
		return ;
	cur = first;
	while (cur)
	{
		i = 0;
		while (i < cur->redir_count)
		{
			if (cur->redirs[i]->temp_file)
			{
				unlink(cur->redirs[i]->temp_file);
				free(cur->redirs[i]->temp_file);
				cur->redirs[i]->temp_file = NULL;
			}
			i++;
		}
		cur = cur->next_cmd;
	}
}
