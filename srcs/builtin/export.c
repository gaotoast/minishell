/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 02:47:53 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/02 21:49:03 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * １つの環境変数を
 * declare -x <NAME>=<VALUE>\n
 * の形で表示する
 */
void	print_env(char *env)
{
	int	i;

	if (ft_strncmp(env, "_=", 2) == 0)
		return ;
	write(STDOUT_FILENO, "declare -x ", 11);
	i = 0;
	while (env[i] && env[i] != '=')
		write(STDOUT_FILENO, &env[i++], 1);
	++i;
	if (env[i])
	{
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, env + i, ft_strlen(env + i));
		write(STDOUT_FILENO, "\"", 1);
	}
	write(STDOUT_FILENO, "\n", 1);
}

int	print_envs(char **envp, int len)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	// ascii順にソート
	while (i < len - 1)
	{
		j = i + 1;
		while (j < len)
		{
			if (ft_strncmp(envp[i], envp[j], ft_strlen(envp[i])) > 0)
			{
				tmp = envp[i];
				envp[i] = envp[j];
				envp[j] = tmp;
			}
			++j;
		}
		++i;
	}
	i = 0;
	while (envp[i])
		print_env(envp[i++]);
	return (0);
}

int	add_bottom_env(char *str, char ***envp, int *len)
{
	char	**rtn;
	int		i;

	rtn = (char **)malloc(sizeof(char *) * (*len + 2));
	if (!rtn)
	{
		perror("minishell");
		return (2);
	}
	i = 0;
	while (i < *len)
	{
		rtn[i] = (*envp)[i];
		++i;
	}
	rtn[i] = str;
	rtn[i + 1] = NULL;
	free(*envp);
	*envp = rtn;
	++(*len);
	return (0);
}

int	overwrite_env(char *str, char ***envp, int idx)
{
	char	*rtn;

	rtn = ft_strdup(str);
	if (!rtn)
	{
		perror("minishell");
		return (2);
	}
	free((*envp)[idx]);
	(*envp)[idx] = rtn;
	return (0);
}

int	get_eq_idx(char *str)
{
	int	i;

	i = 0;
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (-1);
	while (str[i] != '=')
	{
		if (ft_isalnum(str[i]) || str[i] == '_')
			++i;
		else
			return (-1);
	}
	return (i);
}

int	set_input(char *str, char ***envp, int *len)
{
	int	i;
	int	j;
	//TODO エラー処理

	i = get_eq_idx(str);
	j = 0;
	if (i == -1)
		return (1);
	while (j < *len)
	{
		if (ft_strncmp((*envp)[j], str, i + 1))
			++j;
		else
			break ;
	}
	if (j != *len)
		return (overwrite_env(str, envp, j));
	else
		return (add_bottom_env(str, envp, len));
}

int	export(int argc, char **argv, char ***envp)
{
	int	env_len;
	int	i;
	int	rtn;
	int	error;

	env_len = 0;
	while ((*envp)[env_len])
		++env_len;
	if (argc == 1)
		return (print_envs(*envp, env_len));
	i = 1;
	rtn = 0;
	error = 0;
	while (i < argc && argv[i])
	{
		rtn = set_input(argv[i], envp, &env_len);
		if (rtn == 2)
			return (EXIT_FAILURE);
		else if (rtn == 1)
		{
			write(STDERR_FILENO, "minishell: export: `", 15);
			write(STDERR_FILENO, argv[i], strlen(argv[i]));
			write(STDERR_FILENO, "' :not a valid identifier\n", 26);
			error = 1;
		}
		++i;
	}
	if (error)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

// char	**copy_env(char **envp)
// {
// 	int		i;
// 	int		len;
// 	char	**rtn;

// 	len = 0;
// 	while (envp[len])
// 		++len;
// 	rtn = (char **)malloc(sizeof(char *) * (len + 1));
// 	i = 0;
// 	while (envp[i])
// 	{
// 		rtn[i] = strdup(envp[i]);
// 		++i;
// 	}
// 	rtn[i] = NULL;
// 	return (rtn);
// }

// int main(int argc, char **argv, char **envp)
// {
// 	// int i = 0;
// 	// while (argc && argv && envp[i])
// 	// 	printf("%s\n", envp[i++]);
// 	char	**cenv = copy_env(envp);
// 	// while (*cenv)
// 	// 	printf("%s\n", *cenv++);
// 	export(argc, argv, &cenv);
// 	// export(1, argv, &cenv);
// }
