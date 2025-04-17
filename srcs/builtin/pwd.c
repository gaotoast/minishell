/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 01:29:37 by yumiyao           #+#    #+#             */
/*   Updated: 2025/04/18 00:35:17 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_string(char *bufs)
{
	size_t	size;
	char	*rtn;

	size = 512;
	rtn = NULL;
	while (size <= PATH_MAX)
	{
		bufs = malloc(size);
		if (!bufs)
			return (NULL);
		rtn = getcwd(bufs, size);
		if (rtn)
			return (rtn);
		free(bufs);
		if (errno != ERANGE)
			return (NULL);
		size *= 2;
	}
	return (NULL);
}

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
		write(STDERR_FILENO, "minishell: pwd: ", 16);
		write(STDERR_FILENO, argv[1], 2);
		write(STDERR_FILENO, ": invalid option\n", 17);
		return (EXIT_FAILURE);
	}
	bufs = get_string(bufs);
	if (!bufs)
	{
		perror("minishell");
		return (EXIT_FAILURE);
	}
	printf("%s\n", bufs);
	free(bufs);
	return (EXIT_SUCCESS);
}
