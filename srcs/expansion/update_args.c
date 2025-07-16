/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:25:48 by stakada           #+#    #+#             */
/*   Updated: 2025/07/16 15:34:51 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_empty_val_token(t_exp_tkn **head)
{
	t_exp_tkn	*cur;
	t_exp_tkn	*prev;
	t_exp_tkn	*temp;

	prev = NULL;
	cur = *head;
	while (cur)
	{
		if (cur->is_expanded && !cur->is_quoted && cur->str && !cur->str[0])
		{
			temp = cur->next;
			if (prev)
				prev->next = temp;
			else
				*head = temp;
			free(cur->str);
			free(cur);
			cur = temp;
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
}

static int	count_exp_tokens(t_exp_tkn *head)
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

static int	copy_exp_to_array(char **argv, t_exp_tkn *head)
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
			return (-1);
		}
		i++;
		cur = cur->next;
	}
	argv[i] = NULL;
	return (0);
}

static char	**build_array_from_exp(t_exp_tkn *head, int count)
{
	char	**argv;

	argv = (char **)ft_malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	if (copy_exp_to_array(argv, head) != 0)
		return (NULL);
	return (argv);
}

int	update_args_from_exp(t_exp_tkn *head, t_node *node)
{
	remove_empty_val_token(&head);
	node->argc = count_exp_tokens(head);
	node->argv_lst = head;
	free_2d_array(node->argv);
	if (node->argc == 0)
	{
		node->argv = NULL;
		return (0);
	}
	node->argv = build_array_from_exp(node->argv_lst, node->argc);
	if (!node->argv)
		return (-1);
	return (0);
}
