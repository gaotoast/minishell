/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_unsplit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 04:26:02 by yumiyao           #+#    #+#             */
/*   Updated: 2025/07/15 14:10:12 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_unsplit(t_exp_tkn **head, char *str, int env_flag)
{
	t_exp_tkn	*new;
	t_expand	info;

	ft_memset(&info, 0, sizeof(t_expand));
	info.is_quoted = false;
	info.env_flag = env_flag;
	info.state = '\0';
	info.rtn = ft_strdup("");
	while (str[info.i])
	{
		if (info.state == '\0' && state_null(&info, str))
			return (-1);
		else if (info.state == '\'' && state_backquote(&info, str))
			return (-1);
		else if (info.state == '"' && state_doublequote(&info, str))
			return (-1);
	}
	new = new_exp_token(info.rtn, true, info.is_quoted);
	if (!new)
		return (-1);
	append_exp_token(head, new);
	return (0);
}
