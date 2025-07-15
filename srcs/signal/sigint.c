/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigint.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:09:29 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 15:09:30 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int signum)
{
	g_sig_received = signum;
	rl_replace_line("", 0);
	rl_done = 1;
}

void	set_exec_sigint(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: "
			"sigaction: %s\n", strerror(errno));
		inner_exit(1);
	}
}

void	set_main_sigint(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: "
			"sigaction: %s\n", strerror(errno));
		inner_exit(1);
	}
}
