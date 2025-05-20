/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 04:02:23 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/21 06:26:58 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\f' || c == '\n'
			|| c == '\r' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

int	is_numeric(char *s)
{
	int	i;

	i = 0;
	while (ft_isspace(s[i]))
		++i;
	if (s[i] == '+' || s[i] == '-')
		++i;
	while (s[i])
	{
		if (ft_isdigit(s[i]) != 0)
			return (0);
		++i;
	}
	return (1);
}

int	get_digit(char *num, int minus)
{
	int	i;
	int	digit;

	i = 0;
	while (ft_isdigit(num[i]))
		++i;
	digit = i;
	while (ft_isspace(num[i]))
		++i;
	if (num[i] != '\0' || digit > 19)
		return (-1);
	if (digit == 19
		&& ((minus== 1 && ft_strncmp(num, "9223372036854775807", 19) > 0)
		|| (minus == -1 && ft_strncmp(num, "9223372036854775808", 19) > 0)))
		return (-1);
	return (digit);
}

long long int	ft_atol(char *num, long long int minus, int digit)
{
	long long int	rtn;
	int				i;

	rtn = 0;
	i = 0;
	while (i < digit)
	{
		rtn *= 10;
		rtn += num[i] - '0';
		++i;
	}
	rtn *= minus;
	return (rtn);
}

long long int	get_exit_num(char *num)
{
	int				i;
	long long int	minus;
	int				digit;
	long long int	rtn;

	i = 0;
	minus = 1;
	while (ft_isspace(num[i]))
		++i;
	if (num[i] == '-')
	{
		++i;
		minus = -1;
	}
	else if (num[i] == '+')
		++i;
	digit = get_digit(&num[i], minus);
	if (digit == -1)
		return (-1);
	rtn = ft_atol(&num[i], minus, digit);
	rtn %= 256;
	if (rtn < 0)
		return (rtn + 256);
	return (rtn);
}

int	sh_stat(t_st_op op, int val)
{
	static int	status;

	if (op == ST_SET)
		status = val;
	else if (op == ST_GET)
		return (status);
	return (status);
}

int	ft_exit(int argc, char **argv)
{
	int	rtn;

	write(STDIN_FILENO, "exit\n", 5);
	if (argc == 1)
		exit(sh_stat(ST_GET, 0));
	rtn = get_exit_num(argv[1]);
	if (rtn == -1)
	{
		write(STDERR_FILENO, "minishell: exit: ", 17);
		write(STDERR_FILENO, argv[1], ft_strlen(argv[1]));
		write(STDERR_FILENO, ": numeric argument required\n", 28);
		exit(2);
	}
	else if (argc > 2)
	{
		write(STDERR_FILENO, "minishell: exit: too many arguments\n", 37);
		return (EXIT_FAILURE);
	}
	else
		exit(rtn);
	return (EXIT_FAILURE);
}
