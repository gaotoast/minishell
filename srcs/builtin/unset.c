/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 21:58:27 by yumiyao           #+#    #+#             */
/*   Updated: 2025/08/08 21:39:22 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc && argv[i])
	{
		ft_env(ENV_DEL, argv[i]);
		++i;
	}
	return (EXIT_SUCCESS);
}
