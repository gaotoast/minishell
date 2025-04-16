/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 00:20:46 by yumiyao           #+#    #+#             */
/*   Updated: 2025/04/16 22:34:48 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* TODO : libftの使用*/
int	echo(int argc, char **argv)
{
	char	endnew;
	int		i;

	if (argc < 1)
		return (0);
	if (argc >= 2 && strcmp(argv[1], "-n") == 0)
		endnew = 1;
	else
		endnew = 0;
	i = 1 + endnew;
	while (i < argc)
	{
		printf("%s", argv[i++]);
		if (i < argc)
			printf(" ");
	}
	if (!endnew)
		printf("\n");
	return (0);
}

// int main(int argc, char **argv)
// {
// 	echo(argc, argv);
// }
