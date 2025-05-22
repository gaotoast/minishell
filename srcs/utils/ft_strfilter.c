/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strfilter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 21:46:06 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/22 21:51:37 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strfilter(char *str, char c)
{
	int		i;
	int		j;
	int		ctr;
	char	*rtn;

	i = 0;
	ctr = 0;
	while (str[i])
		if (str[i++] == c)
			++ctr;
	rtn = malloc(sizeof(char) * (i - ctr));
	if (!rtn)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != c)
			rtn[j++] = str[i];
		++i;
	}
	return (rtn);
}
