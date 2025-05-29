#include "minishell.h"

void	*ft_malloc(size_t size)
{
	void	*rtn;

	rtn = malloc(size);
	if (!rtn)
	{
		ft_dprintf(STDERR_FILENO, "minishell: malloc: %s\n", strerror(errno));
		return (NULL);
	}
	return (rtn);
}
