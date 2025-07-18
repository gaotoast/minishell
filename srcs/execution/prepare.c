/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:15:31 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 14:16:19 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	prepare_pipe(t_node *node)
{
	if (node->next_cmd)
	{
		if (pipe(node->pipefd) < 0)
		{
			ft_dprintf(STDERR_FILENO, "minishell: pipe: %s\n", strerror(errno));
			return (-1);
		}
	}
	return (0);
}

void	prepare_pipe_child(t_node *node, int count)
{
	if (count > 0)
	{
		if (dup2(node->in_fd, STDIN_FILENO) == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: dup2: %s\n", strerror(errno));
			inner_exit(1);
		}
		close(node->in_fd);
	}
	if (node->next_cmd)
	{
		if (dup2(node->pipefd[1], STDOUT_FILENO) == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: dup2: %s\n", strerror(errno));
			inner_exit(1);
		}
		close(node->pipefd[1]);
		close(node->pipefd[0]);
	}
}

void	prepare_pipe_parent(t_node *node, int count)
{
	if (count > 0)
		close(node->in_fd);
	if (node->next_cmd)
	{
		close(node->pipefd[1]);
		node->next_cmd->in_fd = node->pipefd[0];
	}
}
