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

int	process_expand_cmds(t_exp_tkn **head, char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		// 変数展開とクォート除去
		if (tokenize_with_expansion(head, argv[i], 1) != 0)
		{
			free_exp_tokens(*head);
			return (1);
		}
		i++;
	}
	return (0);
}

// コマンドのargvの展開
int	expand_cmds(t_node *node)
{
	t_exp_tkn	*head;

	head = NULL;
	if (process_expand_cmds(&head, node->argv) != 0)
		return (1);
	// 変数展開の結果を単語分割
	if (split_exp_tokens(&head) != 0)
	{
		free_exp_tokens(head);
		return (1);
	}
	// リストから新しいargvを構築して割当→元のargvを解放
	exp_token_to_argv(head, &node->argv);
	// argcを更新
	node->argc = count_argv(node->argv);
	free_exp_tokens(head);
	return (0);
}
