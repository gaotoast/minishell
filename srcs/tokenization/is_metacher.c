/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_metacher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:35:43 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 14:35:47 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_single_metachar(char *p)
{
	if (*p == '|' || *p == '<' || *p == '>')
		return (1);
	return (0);
}

int	is_two_metachar(char *p)
{
	if (ft_strncmp(p, "<<", 2) == 0 || ft_strncmp(p, ">>", 2) == 0)
		return (1);
	return (0);
}
