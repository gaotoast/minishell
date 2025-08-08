/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 01:29:37 by yumiyao           #+#    #+#             */
/*   Updated: 2025/08/08 21:39:25 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_pwd_error(int argc, char **argv)
{
	if (argc == 1)
		return (0);
	if (argv[1][0] != '-' || ft_strncmp(argv[1], "--", 3) == 0)
		return (0);
	if (argv[1][1] == '\0')
		return (0);
	return (1);
}

int	pwd(int argc, char **argv)
{
	char	*bufs;

	bufs = NULL;
	if (is_pwd_error(argc, argv))
	{
		ft_dprintf(STDERR_FILENO, "minishell: pwd: %c%c: "
			"invalid option\n", argv[1][0], argv[1][1]);
		return (2);
	}
	bufs = ft_cwd(PWD_GET, NULL);
	ft_dprintf(STDOUT_FILENO, "%s\n", bufs);
	return (EXIT_SUCCESS);
}
