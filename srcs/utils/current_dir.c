/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   current_dir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 04:26:40 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/21 04:30:43 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getcwd(t_pwd_op op, char *path)
{
	static char	*pwd;

	if (op == PWD_SET)
		pwd = path;
	else if (op == PWD_GET)
		return (pwd);
	return (pwd);
}
