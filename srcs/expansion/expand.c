#include "minishell.h"

int	expand(t_node *node)
{
	if (!node)
		return (0);
	if (node->kind == ND_CMD)
	{
		if (node->argv)
		{
			if (expand_cmds(node) != 0)
				return (1);
		}
		if (node->redirs)
		{
			if (expand_redirs(node) != 0)
				return (1);
		}
		return (0);
	}
	if (expand(node->lhs) != 0)
		return (1);
	if (expand(node->rhs) != 0)
		return (1);
	return (0);
}
