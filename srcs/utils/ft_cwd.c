/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 04:26:40 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/22 15:10:22 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_cwd(t_pwd_op op, char *path)
{
	static char	*pwd;

	if (op == PWD_SET)
	{
		free(pwd);
		pwd = path;
	}
	else if (op == PWD_GET)
		return (pwd);
	return (pwd);
}
