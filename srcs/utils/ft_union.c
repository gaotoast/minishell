/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_union.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 14:02:36 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/22 14:03:00 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_union(char **split, char delim)
{
	int		i;
	int		tmp_len;
	int		len;
	char	*rtn;

	i = 0;
	len = 0;
	if (!split)
		return (NULL);
	while (split[i])
		len += ft_strlen(split[i++]);
	rtn = malloc(sizeof(char) * (len + i + 1));
	if (!rtn)
		return (NULL);
	i = 0;
	len = 0;
	while (split[i])
	{
		rtn[len++] = delim;
		tmp_len = strlen(split[i]);
		ft_strlcpy(&(rtn[len]), split[i++], tmp_len + 1);
		len += tmp_len;
	}
	return (rtn);
}
