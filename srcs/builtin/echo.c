/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 00:20:46 by yumiyao           #+#    #+#             */
/*   Updated: 2025/06/01 14:08:58 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_n_option(char *opt)
{
	int	i;

	if (opt[0] != '-')
		return (0);
	i = 1;
	while (opt[i])
	{
		if (opt[i] != 'n')
			return (0);
		++i;
	}
	return (1);
}

t_exp_tkn	*check_argv(t_exp_tkn *argv_lst, int *endnew)
{
	t_exp_tkn	*tmp;
	t_exp_tkn	*print_head;

	*endnew = 1;
	tmp = argv_lst;
	print_head = tmp;
	while (tmp)
	{
		if (is_n_option(tmp->str))
		{
			print_head = tmp->next;
			*endnew = 0;
		}
		tmp = tmp->next;
	}
	return (print_head);
}

int	echo(int argc, t_exp_tkn *argv_lst)
{
	int			endnew;
	t_exp_tkn	*tmp;
	int			i;

	if (argc < 1)
		return (EXIT_SUCCESS);
	tmp = check_argv(argv_lst, &endnew);
	while (i < argc)
	{
		ft_dprintf(STDOUT_FILENO, "%s", tmp->str);
		if (tmp->next)
			ft_dprintf(STDOUT_FILENO, " ");
		tmp = tmp->next;
	}
	if (endnew)
		ft_dprintf(STDOUT_FILENO, "\n");
	return (EXIT_SUCCESS);
}

// int main(int argc, char **argv)
// {
// 	echo(argc, argv);
// }
