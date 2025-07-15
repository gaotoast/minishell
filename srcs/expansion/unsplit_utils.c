/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsplit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 09:07:50 by yumiyao           #+#    #+#             */
/*   Updated: 2025/07/11 09:14:52 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*connect_char(char **s1, char c)
{
	char	*rtn;
	int		len;

	len = ft_strlen(*s1) + 2;
	rtn = (char *)ft_malloc(sizeof(char) * len);
	if (!rtn)
		return (NULL);
	ft_strlcpy(rtn, *s1, len);
	rtn[len - 2] = c;
	rtn[len - 1] = '\0';
	free(*s1);
	*s1 = rtn;
	return (rtn);
}

static char	*process_inner_dollar(char *s, char *result, int *idx)
{
	char	*name;
	char	*ret;
	char	*value;

	if (extract_var_name(s, &name) != 0)
	{
		free(result);
		return (NULL);
	}
	if (!name)
	{
		ret = ft_strjoin(result, "$");
		free(result);
		return (ret);
	}
	*idx += ft_strlen(name);
	value = get_var_value(name);
	free(name);
	if (!value)
		return (NULL);
	ret = ft_strjoin(result, value);
	free(result);
	free(value);
	return (ret);
}

int	state_null(t_expand *info, char *str)
{
	if (str[info->i] == '\'')
	{
		info->state = '\'';
		++(info->i);
	}
	else if (str[info->i] == '"')
	{
		info->is_quoted = true;
		info->state = '"';
		++(info->i);
	}
	else if (str[info->i] == '$' && info->env_flag)
	{
		++(info->i);
		//$以降の処理
		info->rtn = process_inner_dollar(&str[info->i], info->rtn, &(info->i));
	}
	else
		if (!connect_char(&(info->rtn), str[(info->i)++]))
		{
			free(info->rtn);
			return (1);
		}
	return (0);
}

int	state_backquote(t_expand *info, char *str)
{
	if (str[info->i] == '\'')
	{
		info->state = '\0';
		++(info->i);
	}
	else
		if (!connect_char(&(info->rtn), str[(info->i)++]))
		{
			free(info->rtn);
			return (1);
		}
	return (0);
}

int	state_doublequote(t_expand *info, char *str)
{
	if (str[info->i] == '"')
	{
		info->state = '\0';
		++(info->i);
	}
	else if (str[info->i] == '$' && info->env_flag)
	{
		++(info->i);
		//$以降の処理
		info->rtn = process_inner_dollar(&str[info->i], info->rtn, &(info->i));
	}
	else
		if (!connect_char(&(info->rtn), str[(info->i)++]))
		{
			free(info->rtn);
			return (1);
		}
	return (0);
}
