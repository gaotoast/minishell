#include "minishell.h"

void	print_exec_list(t_node *node)
{
	while (node)
	{
		printf("CMD: ");
		for (int i = 0; i < node->argc; i++)
			printf("%s ", node->argv[i]);
		printf("\n");
		node = node->next_cmd;
	}
}

void	debug_exec_list(t_node *node)
{
    printf("===EXEC LIST===\n");
    if (!node)
    {
        printf("No node.\n");
        return ;
    }
    print_exec_list(node);
    printf("===EXEC LIST END===\n");
}
