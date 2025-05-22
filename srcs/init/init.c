#include "minishell.h"

char	**get_envp_copy(char **envp)
{
	char	**cp;
	int		env_count;
	int		i;

	env_count = 0;
	while (envp[env_count])
		env_count++;
	cp = (char **)malloc(sizeof(char *) * (env_count + 1));
	if (!cp)
	{
		perror("minishell");
		return (NULL);
	}
	i = 0;
	while (i < env_count)
	{
		cp[i] = ft_strdup(envp[i]);
		if (!cp[i])
		{
			while (--i >= 0)
				free(cp[i]);
			free(cp);
			return (NULL);
		}
		i++;
	}
	cp[i] = NULL;
	return (cp);
}

int	init_pwd(char **envp)
{
	char	*pwd;

	pwd = ft_strdup(ft_getenv("PWD", envp));
	if (!pwd)
	{
		pwd = malloc(sizeof(char) * (PATH_MAX + 1));
		if (!pwd)
			return (1);
		getcwd(pwd, PATH_MAX);
	}
	ft_cwd(PWD_SET, pwd);
	return (0);
}

int	pure_atoi(char *num)
{
	int	rtn;
	int	i;
	int	sign;

	rtn = 0;
	i = 0;
	sign = 1;
	if (ft_isspace(num[i]))
		++i;
	if (num[i] == '-')
	{
		sign = -1;
		++i;
	}
	else if (num[i] == '+')
		++i;
	while (ft_isdigit(num[i]))
	{
		rtn *= 10;
		rtn += num[i] - '0';
		++i;
	}
	return (rtn);
}

int	is_valid_shlvl(char *num)
{
	int	i;

	i = 0;
	while (ft_isspace(num[i]))
		++i;
	if (num[i] == '+' || num[i] == '-')
		++i;
	while (ft_isdigit(num[i]))
		++i;
	while (ft_isspace(num[i]))
		++i;
	if (num[i] == '\0')
		return (0);
	return (1);
}

void	init_shlvl(char ***envp)
{
	char	*shlvl;
	int		lvl;
	int		len;

	shlvl = ft_getenv("SHLVL", *envp);
	len = ft_split_len(*envp);
	if (!shlvl)
	{
		set_env("SHLVL=1", envp, &len);
		return ;
	}
	if (is_valid_shlvl(shlvl))
	{
		set_env("SHLVL=1", envp, &len);
		return ;
	}
	lvl = pure_atoi(shlvl);
	if (lvl > 998)
	{
		printf("minishell: warning: shell level (%d) "
			"too high, resetting to 1\n", lvl + 1);
		set_env("SHLVL=1", envp, &len);
	}
	else if (lvl < 0)
		set_env("SHLVL=0", envp, &len);
	else
		set_env(ft_strjoin("SHLVL=", ft_itoa(lvl + 1)), envp, &len);
}

int	init(t_shell **shell, char **envp)
{
	(*shell) = (t_shell *)malloc(sizeof(t_shell));
	if (!(*shell))
	{
		perror("minishell");
		return (-1);
	}
	sh_stat(ST_SET, 0);
	(*shell)->tokens = NULL;
	(*shell)->envp_cp = get_envp_copy(envp);
	if (!(*shell)->envp_cp)
	{
		free(*shell);
		return (-1);
	}
	if (init_pwd((*shell)->envp_cp))
	{
		free_2d_array((*shell)->envp_cp);
		return (-1);
	}
	init_shlvl(&(*shell)->envp_cp);
	(*shell)->ast = NULL;
	return (0);
}
