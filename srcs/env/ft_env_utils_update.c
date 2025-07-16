/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils_update.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 23:53:57 by yumiyao           #+#    #+#             */
/*   Updated: 2025/07/16 14:51:14 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_val(t_env *target, char *str, int offset, char **tmp)
{
	char	*val;

	val = ft_strchr(str, '=');
	if (!val)
		return (1);
	if (offset && target->type == VAL_EX)
		*tmp = rm_quotes(ft_strjoin(target->val, val));
	else
	{
		target->type = VAL_EX;
		*tmp = rm_quotes(ft_strdup(val));
	}
	return (0);
}

t_env	*ft_update_env(t_env **head, char *str, int offset)
{
	t_env	*target;
	char	*tmp;
	int		check;

	target = search_val(*head, str, offset, VAL_SH);
	if (!target)
		return (ft_add_env(head, str, offset));
	check = check_val(target, str, offset, &tmp);
	if (check)
		return (*head);
	if (!tmp)
		return (NULL);
	free(target->val);
	target->val = tmp;
	tmp = ft_strjoin_delim(target->name, '=', target->val);
	if (!tmp)
		return (NULL);
	free(target->full);
	target->full = tmp;
	return (*head);
}
