/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inner_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 21:09:05 by yumiyao           #+#    #+#             */
/*   Updated: 2025/07/13 12:07:04 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	inner_exit(int status)
{
	rl_clear_history();
	ft_env(ENV_DEL_ALL, NULL);
	sh_op(SH_DEL, NULL);
	exit(status);
}
