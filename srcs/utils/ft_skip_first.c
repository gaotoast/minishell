/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_skip_first.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 21:46:06 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/25 04:54:50 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_skip_first(char *str, char c)
{
	int		i;
	int		j;
	int		skip;
	char	*rtn;

	i = 0;
	skip = 0;
	while (str[i])
	{
		if (!skip && str[i] == c)
			skip = 1;
		++i;
	}
	rtn = malloc(sizeof(char) * (i - skip));
	if (!rtn)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == c && skip)
			skip = 0;
		else
			rtn[j++] = str[i];
		++i;
	}
	return (rtn);
}
