#include "minishell.h"

void	print_exp_token_list(t_exp_tkn *head)
{
	int	index = 0;

	printf("=== Expansion Token List ===\n");
	if (!head)
		puts("(null)");
	while (head)
	{
		printf("[%d] str: \"%s\", is_expanded: %s\n",
			index,
			head->str ? head->str : "(null)",
			head->is_expanded ? "true" : "false");
		head = head->next;
		index++;
	}
	printf("============================\n");
}
