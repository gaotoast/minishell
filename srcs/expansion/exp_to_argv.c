#include "minishell.h"

// 空でない文字列を数える
int	count_non_empty_tokens(t_exp_tkn *head)
{
	int			count;
	t_exp_tkn	*cur;

	count = 0;
	cur = head;
	while (cur)
	{
		if (cur->str[0] != '\0')
			count++;
		cur = cur->next;
	}
	return (count);
}

int	copy_exp_to_argv(char **argv, t_exp_tkn *head)
{
	t_exp_tkn	*cur;
	int			i;

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
				return (1);
			}
			i++;
		}
		cur = cur->next;
	}
	argv[i] = NULL;
	return (0);
}

char	**build_argv_from_exp(t_exp_tkn *head, int count)
{
	char	**argv;

	argv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!argv)
	{
		perror("minishell: malloc");
		return (NULL);
	}
	if (copy_exp_to_argv(argv, head) != 0)
		return (NULL);
	return (argv);
}

// 空文字列以外の文字列からargvを構築
char	**exp_token_to_argv(t_exp_tkn *head)
{
	int		count;
	char	**argv;

	count = count_non_empty_tokens(head);
	if (count == 0)
		return (NULL);
	argv = build_argv_from_exp(head, count);
	return (argv);
}
