#include "minishell.h"

// 新しいexpトークンを作成
t_exp_tkn	*new_exp_token(char *str, bool is_expanded)
{
	t_exp_tkn	*new;

	new = (t_exp_tkn *)malloc(sizeof(t_exp_tkn));
	if (!new)
	{
		perror("minishell: malloc");
		return (NULL);
	}
	new->str = str;
	new->is_expanded = is_expanded;
	new->next = NULL;
	return (new);
}

// リストにexpトークンを追加
void	append_exp_token(t_exp_tkn **head, t_exp_tkn *new)
{
	t_exp_tkn	*cur;

	if (!*head)
	{
		*head = new;
		return ;
	}
	cur = *head;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

// リストを解放
void	free_exp_tokens(t_exp_tkn *head)
{
	t_exp_tkn	*cur;
	t_exp_tkn	*next;

	if (!head)
		return ;
	cur = head;
	while (cur)
	{
		next = cur->next;
		free(cur->str);
		free(cur);
		cur = next;
	}
}

// 空文字列以外の文字列からargvを構築
char	**exp_token_to_argv(t_exp_tkn *head)
{
	int			count;
	t_exp_tkn	*cur;
	char		**argv;
	int			i;

	count = 0;
	cur = head;
	while (cur)
	{
		if (cur->str[0] != '\0')
			count++;
		cur = cur->next;
	}
	if (count == 0)
		return (NULL);
	argv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	cur = head;
	i = 0;
	while (cur)
	{
		if (cur->str[0] != '\0')
		{
			argv[i] = ft_strdup(cur->str);
			if (!argv[i])
			{
				free_2d_array(argv);
				return (NULL);
			}
			i++;
		}
		cur = cur->next;
	}
	argv[i] = NULL;
	return (argv);
}

