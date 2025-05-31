#include "minishell.h"

// 文字列の数を数える
int	count_exp_tokens(t_exp_tkn *head)
{
	int			count;
	t_exp_tkn	*cur;

	count = 0;
	cur = head;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	return (count);
}

// リストから新しい二次元配列に文字列をコピー
int	copy_exp_to_argv(char **argv, t_exp_tkn *head)
{
	t_exp_tkn	*cur;
	int			i;

	cur = head;
	i = 0;
	while (cur)
	{
		argv[i] = ft_strdup(cur->str);
		if (!argv[i])
		{
			free_2d_array(argv);
			return (1);
		}
		i++;
		cur = cur->next;
	}
	argv[i] = NULL;
	return (0);
}

// リストから新しい二次元配列を構築
char	**build_argv_from_exp(t_exp_tkn *head, int count)
{
	char	**argv;

	argv = (char **)ft_malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	if (copy_exp_to_argv(argv, head) != 0)
		return (NULL);
	return (argv);
}

// 空文字列以外の文字列からargvを構築
int	exp_token_to_argv(t_exp_tkn *head, char ***argv)
{
	int	count;

	count = count_exp_tokens(head);
	free_2d_array(*argv);
	if (count == 0)
	{
		*argv = NULL;
		return (0);
	}
	*argv = build_argv_from_exp(head, count);
	return (0);
}
