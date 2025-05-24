#include "minishell.h"

int	process_expand_cmds(t_exp_tkn **head, char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		// 変数展開とクォート除去
		if (tokenize_with_expansion(head, argv[i]) != 0)
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

int	process_expand_redirs(t_exp_tkn **head, char *str)
{
	int			count;
	t_exp_tkn	*cur;

	// 変数展開の結果を単語分割
	if (split_exp_tokens(head) != 0)
	{
		free_exp_tokens(*head);
		return (1);
	}
	// 変数展開→単語分割後の文字列の数を数える
	count = 0;
	cur = *head;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	// 文字列が複数ならambiguous redirectエラー
	if (count != 1)
	{
		ft_dprintf(STDERR_FILENO, "minishell: %s: ambiguous redirect\n", str);
		free_exp_tokens(*head);
		return (1);
	}
	return (0);
}

// リダイレクトのファイル名の展開
int	expand_redirs(t_node *node)
{
	int			i;
	t_exp_tkn	*new;

	i = 0;
	while (node->redirs[i])
	{
		new = NULL;
		if (node->redirs[i]->kind != REDIR_HEREDOC)
		{
			// 変数展開とクォート除去
			if (tokenize_with_expansion(&new, node->redirs[i]->str) != 0)
			{
				free_exp_tokens(new);
				return (1);
			}
			if (process_expand_redirs(&new, node->redirs[i]->str) != 0)
				return (1);
			// 新しい文字列を割当→元のstrを解放
			free(node->redirs[i]->str);
			node->redirs[i]->str = new->str;
			free(new);
		}
		i++;
	}
	return (0);
}

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
