/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 20:58:51 by yumiyao           #+#    #+#             */
/*   Updated: 2025/07/15 15:06:04 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long long int	ft_atol(char *num, long long int minus, int digit)
{
	long long int	rtn;
	int				i;

	rtn = 0;
	i = 0;
	while (i < digit)
	{
		rtn *= 10;
		rtn += num[i] - '0';
		++i;
	}
	rtn *= minus;
	return (rtn);
}
