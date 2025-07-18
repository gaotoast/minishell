/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:03:41 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 15:03:44 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sh_stat(t_st_op op, int val)
{
	static int	status;
	int			ret;

	if (op == ST_SET)
		status = val;
	else if (op == ST_GET)
	{
		if (g_sig_received)
		{
			ret = g_sig_received + 128;
			g_sig_received = 0;
			return (ret);
		}
		else
		{
			ret = status;
			status = 0;
			return (ret);
		}
	}
	return (status);
}
