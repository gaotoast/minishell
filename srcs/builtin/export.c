/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 02:47:53 by yumiyao           #+#    #+#             */
/*   Updated: 2025/05/22 19:49:08 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (!str)
		return (-1);
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

int	set_env(char *str, char ***envp, int *len)
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
		rtn = set_env(argv[i], envp, &env_len);
		if (rtn == 2)
			return (EXIT_FAILURE);
		else if (rtn == 1)
		{
			write(STDERR_FILENO, "minishell: export: `", 20);
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
