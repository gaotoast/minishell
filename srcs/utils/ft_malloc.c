/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:03:07 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 15:03:08 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_malloc(size_t size)
{
	void	*rtn;

	rtn = malloc(size);
	if (!rtn)
	{
		ft_dprintf(STDERR_FILENO, "minishell: malloc: %s\n", strerror(errno));
		return (NULL);
	}
	return (rtn);
}
