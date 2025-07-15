/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_temp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:08:48 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 15:08:49 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_temp_file(int index, char **temp_file)
{
	char	*index_str;
	size_t	path_len;

	index_str = ft_itoa(index);
	if (!index_str)
		return (1);
	path_len = ft_strlen(HEREDOC_TMP) + ft_strlen(index_str) + 1;
	if (path_len >= PATH_MAX)
	{
		free(index_str);
		return (1);
	}
	*temp_file = ft_strjoin(HEREDOC_TMP, index_str);
	free(index_str);
	if (!(*temp_file))
		return (1);
	return (0);
}
