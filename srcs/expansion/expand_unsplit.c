/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_unsplit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 04:26:02 by yumiyao           #+#    #+#             */
/*   Updated: 2025/07/10 05:12:09 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static char	*handle_dquote(char *str, char *state)
// {
// 	if (state == '\0')
// 		*state = '"';
// 	else if
// }

char *connect_char(char *s1, char c)
{
	char	*rtn;
	int		len;

	len = ft_strlen(s1) + 2;
	rtn = (char *)ft_malloc(sizeof(char) * len);
	if (!rtn)
		return (NULL);
	ft_strlcpy(rtn, s1, len);
	rtn[len - 2] = c;
	rtn[len - 1] = '\0';
	printf("connect %s\n", rtn);
	return (rtn);
}

char	*process_inner_dollar(char *s, char *result, int *idx)
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

int	expand_unsplit(t_exp_tkn **head, char *str, int env_flag)
{
	int		i;
	bool	is_quoted;
	char	state;
	char	*rtn;
	char	*tmp;
	t_exp_tkn	*new;

	i = 0;
	is_quoted = false;
	state = '\0';
	rtn = ft_strdup("");
	while (str[i])
	{
		if (state == '\0')
		{
			// printf("state null\n");
			if (str[i] == '\'')
			{
				// printf("null of '\n");
				state = '\'';
				++i;
			}
			else if (str[i] == '"')
			{
				// printf("null of \"\n");
				is_quoted = true;
				state = '"';
				++i;
			}
			else if (str[i] == '$' && env_flag)
			{
				++i;
				//$以降の処理
				// printf("null of $\n");
				rtn = process_inner_dollar(&str[i], rtn, &i);
			}
			else
			{
				printf("null of other\n");
				//一文字追加
				tmp = connect_char(rtn, str[i]);
				free(rtn);
				if (!tmp)
					return (1);
				rtn = tmp;
				++i;
			}
		}
		else if (state == '\'')
		{
			printf("state '\n");
			if (str[i] == '\'')
			{
				state = '\0';
				++i;
			}
			else
			{
				//一文字追加
				tmp = connect_char(rtn, str[i]);
				free(rtn);
				if (!tmp)
					return (1);
				rtn = tmp;
				++i;
			}
		}
		else if (state == '"')
		{
			printf("state \"\n");
			if (str[i] == '"')
			{
				state = '\0';
				++i;
			}
			else if (str[i] == '$' && env_flag)
			{
				++i;
				//$以降の処理
				rtn = process_inner_dollar(&str[i], rtn, &i);
			}
			else
			{
				//一文字追加
				tmp = connect_char(rtn, str[i]);
				free(rtn);
				if (!tmp)
					return (1);
				rtn = tmp;
				++i;
			}
		}
	}
	new = new_exp_token(rtn, true, is_quoted);
	append_exp_token(head, new);
	return (0);
}
