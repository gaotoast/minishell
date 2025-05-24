#include "minishell.h"

void	print_2d_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
	{
		printf("(null)\n");
		return ;
	}
	while (array[i])
	{
		printf("[%d]: %s\n", i, array[i]);
		i++;
	}
}
