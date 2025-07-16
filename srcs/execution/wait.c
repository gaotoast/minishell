/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:16:31 by stakada           #+#    #+#             */
/*   Updated: 2025/07/16 16:50:36 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_exit_status(int w_status)
{
	int	status;

	if (((((w_status) & 0x7f) + 1) >> 1) > 0)
	{
		status = 128 + ((w_status) & 0x7f);
		if (status == 128 + SIGINT)
		{
			g_sig_received = SIGINT;
			write(STDOUT_FILENO, "\n", 1);
		}
		else if (status == 128 + SIGQUIT)
		{
			g_sig_received = SIGQUIT;
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		}
	}
	else
	{
		status = ((w_status) & 0xff00) >> 8;
		g_sig_received = 0;
	}
	return (status);
}

int	wait_children(pid_t last_pid, t_node *first_cmd)
{
	pid_t	wait_ret;
	int		w_status;
	int		status;

	status = 0;
	while (1)
	{
		wait_ret = wait(&w_status);
		if (wait_ret == last_pid)
		{
			status = process_exit_status(w_status);
			break ;
		}
		else if (wait_ret < 0)
		{
			if (errno == ECHILD)
				break ;
		}
	}
	unlink_all_temp(first_cmd);
	return (status);
}
