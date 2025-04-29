#include "minishell.h"

char	*ft_getenv(char *name, char **envp)
{
	size_t	name_len;
	int		i;

	if (!envp)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if ((ft_strncmp(name, envp[i], name_len) == 0)
			&& envp[i][name_len] == '=')
			return (&envp[i][name_len + 1]);
		i++;
	}
	return (NULL);
}
