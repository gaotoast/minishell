/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 00:20:46 by yumiyao           #+#    #+#             */
/*   Updated: 2025/06/01 15:33:40 by yumiyao          ###   ########.fr       */
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
	tmp = argv_lst->next;
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

void	print_token(t_exp_tkn *token)
{
	if (!token->str[0] && token->is_expanded && !token->is_quoted)
		return ;
	ft_dprintf(STDOUT_FILENO, "%s", token->str);
	if (token->next)
		ft_dprintf(STDOUT_FILENO, " ");
}

int	echo(int argc, t_exp_tkn *argv_lst)
{
	int			endnew;
	t_exp_tkn	*tmp;

	if (argc < 1 || !argv_lst)
		return (EXIT_SUCCESS);
	tmp = check_argv(argv_lst, &endnew);
	while (tmp)
	{
		print_token(tmp);
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
