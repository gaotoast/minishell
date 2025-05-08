#include "minishell.h"

t_node	*get_leftmost_node(t_node *node)
{
	while (node && node->kind == ND_PIPE)
		node = node->lhs;
	return (node);
}

void	print_pipeline(t_node *start)
{
	t_node	*cur;
	int		index;
	t_redir	*r;

	cur = start;
	index = 0;
	while (cur)
	{
		printf("[%d]\n", index);
		if (cur->argc > 0)
		{
			printf("  argv: ");
			for (int i = 0; i < cur->argc; i++)
				printf("%s ", cur->argv[i]);
			printf("\n");
		}
		else
		{
			printf("  (no argv)\n");
		}
		// リダイレクトも表示
		if (cur->redir_count > 0)
		{
			printf("  redirections:\n");
			for (int i = 0; i < cur->redir_count; i++)
			{
				r = cur->redirs[i];
				printf("    kind: %d, target: %s\n", r->kind, r->str);
			}
		}
		cur = cur->next_cmd;
		index++;
	}
}

void	debug_cmd_ptr(t_node *root)
{
	t_node	*leftmost;

	leftmost = get_leftmost_node(root);
	printf("===CMD CHAIN===\n");
	print_pipeline(leftmost);
	printf("===CMD CHAIN END===\n");
}
