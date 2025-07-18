/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:26:19 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 14:26:20 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*append_char(char *result, char c)
{
	char	*temp;
	int		len;

	len = ft_strlen(result);
	temp = (char *)ft_malloc(len + 2);
	if (!temp)
	{
		free(result);
		return (NULL);
	}
	ft_strlcpy(temp, result, len + 1);
	temp[len] = c;
	temp[len + 1] = '\0';
	free(result);
	return (temp);
}

int	expand_heredoc_line(char **line)
{
	char	*result;
	char	*s;

	result = ft_strdup("");
	if (!result)
		return (1);
	s = *line;
	while (*s)
	{
		if (*s == '$')
			result = process_dollar(&s, result);
		else
		{
			result = append_char(result, *s);
			s++;
		}
		if (!result)
			return (1);
	}
	free(*line);
	*line = result;
	return (0);
}
