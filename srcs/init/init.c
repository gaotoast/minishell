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
	(*shell)->ast = NULL;
	return (0);
}
