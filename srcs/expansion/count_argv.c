#include "minishell.h"

int	count_argv(char **argv)
{
	int	count;

	if (!argv)
		return (0);
	count = 0;
	while (argv[count])
		count++;
	return (count);
}
