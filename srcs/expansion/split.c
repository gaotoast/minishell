#include "minishell.h"

// 空白文字（スペースまたはタブ文字）で単語分割
t_exp_tkn	*split_by_blank(char *s)
{
	t_exp_tkn	*head;
	t_exp_tkn	*new;
	int			i;
	int			start;
	char		*str;

	head = NULL;
	new = NULL;
	i = 0;
	while (s[i])
	{
		while (s[i] && is_blank(s[i]))
			i++;
		start = i;
		while (s[i] && !is_blank(s[i]))
			i++;
		if (i > start)
			str = ft_strndup(&s[start], i - start);
		else
			str = ft_strdup("");
		if (!str)
		{
			free_exp_tokens(head);
			return (NULL);
		}
		new = new_exp_token(str, true);
		if (!new)
		{
			free_exp_tokens(head);
			return (NULL);
		}
		append_exp_token(&head, new);
	}
	return (head);
}

// 変数展開の結果を単語分割
int	split_expanded_tokens(t_exp_tkn **head)
{
	t_exp_tkn	*cur;
	t_exp_tkn	*prev;
	t_exp_tkn	*split;
	t_exp_tkn	*tail;

	cur = *head;
	prev = NULL;
	while (cur)
	{
		if (cur->is_expanded && (ft_strchr(cur->str, ' ') || ft_strchr(cur->str,
					'\t')))
		{
			split = split_by_blank(cur->str);
			if (!split)
				return (1);
			tail = split;
			while (tail->next)
				tail = tail->next;
			if (prev)
				prev->next = split;
			else
				*head = split;
			tail->next = cur->next;
			free(cur->str);
			free(cur);
			cur = tail->next;
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
	return (0);
}

