#include "minishell.h"

// コマンドのargvの展開
void	expand_cmds(t_node *node, char **envp)
{
	int			i;
	t_exp_tkn	*head;
	char		**new_argv;

	i = 0;
	head = NULL;
	while (node->argv[i])
	{
		// 変数展開とクォート除去
		if (tokenize_with_expansion(&head, node->argv[i], envp) != 0)
		{
			free_exp_tokens(head);
			sh_stat(ST_SET, 1);
			return ;
		}
		i++;
	}
	// 変数展開の結果を単語分割
	if (split_expanded_tokens(&head) != 0)
	{
		free_exp_tokens(head);
		sh_stat(ST_SET, 1);
		return ;
	}
	// リストから新しいargvを構築して割当→元のargvを解放
	new_argv = exp_token_to_argv(head);
	free_exp_tokens(head);
	if (!new_argv)
	{
		sh_stat(ST_SET, 1);
		return ;
	}
	free_2d_array(node->argv);
	node->argv = new_argv;
}

// リダイレクトのファイル名の展開
void	expand_redirs(t_node *node, char **envp)
{
	int			i;
	t_exp_tkn	*new;
	int			count;
	t_exp_tkn	*cur;

	i = 0;
	while (node->redirs[i])
	{
		new = NULL;
		if (node->redirs[i]->kind != REDIR_HEREDOC)
		{
			// 変数展開とクォート除去
			if (tokenize_with_expansion(&new, node->redirs[i]->str, envp) != 0)
			{
				free_exp_tokens(new);
				sh_stat(ST_SET, 1);
				return ;
			}
			// 変数展開の結果を単語分割
			if (split_expanded_tokens(&new) != 0)
			{
				free_exp_tokens(new);
				sh_stat(ST_SET, 1);
				return ;
			}
			// 文字列が複数ならambiguous redirectエラー
			count = 0;
			cur = new;
			while (cur)
			{
				count++;
				cur = cur->next;
			}
			if (count != 1)
			{
				ft_dprintf(STDERR_FILENO, "minishell: %s: ambiguous redirect\n",
					node->redirs[i]->str);
				free_exp_tokens(new);
				sh_stat(ST_SET, 1);
				return ;
			}
			// 新しい文字列を割当→元のstrを解放
			free(node->redirs[i]->str);
			node->redirs[i]->str = new->str;
			free(new); // strは移譲済み
		}
		i++;
	}
}

// FIXME: どこかでstat間違えて設定してそう

void	expand(t_node *node, char **envp)
{
	if (!node)
		return ;
	if (node->kind == ND_CMD)
	{
		if (node->argv)
			expand_cmds(node, envp);
		if (node->redirs)
			expand_redirs(node, envp);
		return ;
	}
	expand(node->lhs, envp);
	// if (sh_stat(ST_GET, 0) != 0)
	// 	return ;
	expand(node->rhs, envp);
	// if (sh_stat(ST_GET, 0) != 0)
	// return ;
}
