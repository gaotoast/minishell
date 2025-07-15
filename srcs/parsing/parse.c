/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:06:48 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 15:07:34 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO: 分割
t_redir	*parse_redir(t_token **rest, int *ret)
{
	t_redir	*redir;

	redir = ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (NULL);
	if (consume_reserved(rest, ">>"))
		redir->kind = REDIR_APPEND;
	else if (consume_reserved(rest, ">"))
		redir->kind = REDIR_OUT;
	else if (consume_reserved(rest, "<<"))
		redir->kind = REDIR_HEREDOC;
	else if (consume_reserved(rest, "<"))
		redir->kind = REDIR_IN;
	else
	{
		free(redir);
		*ret = -1;
		return (NULL);
	}
	redir->temp_file = NULL;
	*ret = consume_word(rest, &redir->str);
	if (*ret != 0)
	{
		free(redir);
		return (NULL);
	}
	return (redir);
}

// TODO: 分割
t_node	*parse_command(t_token **rest, int *ret)
{
	t_token	*cur;
	t_node	*node;
	char	**argv_tmp;
	t_redir	**redirs_tmp;
	int		i;

	cur = *rest;
	node = new_command_node();
	if (!node)
	{
		*ret = -1;
		return (NULL);
	}
	while (cur)
	{
		if (peek_word(cur))
		{
			node->argc++;
			argv_tmp = (char **)ft_malloc(sizeof(char *) * (node->argc + 1));
			if (!argv_tmp)
			{
				free_ast(node);
				*ret = -1;
				return (NULL);
			}
			i = 0;
			while (i < node->argc - 1)
			{
				argv_tmp[i] = node->argv[i];
				i++;
			}
			argv_tmp[node->argc - 1] = ft_strdup(cur->str);
			if (!argv_tmp[node->argc - 1])
			{
				free_ast(node);
				free(argv_tmp);
				*ret = -1;
				return (NULL);
			}
			argv_tmp[node->argc] = NULL;
			free(node->argv);
			node->argv = argv_tmp;
			cur = cur->next;
		}
		else if (peek_redir_op(cur))
		{
			node->redir_count++;
			redirs_tmp = (t_redir **)ft_malloc(sizeof(t_redir *)
					* (node->redir_count + 1));
			if (!redirs_tmp)
			{
				free_ast(node);
				*ret = -1;
				return (NULL);
			}
			i = 0;
			while (i < node->redir_count - 1)
			{
				redirs_tmp[i] = node->redirs[i];
				i++;
			}
			redirs_tmp[node->redir_count - 1] = parse_redir(&cur, ret);
			if (!redirs_tmp[node->redir_count - 1])
			{
				free_ast(node);
				free(redirs_tmp);
				return (NULL);
			}
			redirs_tmp[node->redir_count] = NULL;
			free(node->redirs);
			node->redirs = redirs_tmp;
		}
		else
			break ;
	}
	if (node->argc == 0 && node->redir_count == 0)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
		free_ast(node);
		*ret = 2;
		return (NULL);
	}
	*rest = cur;
	return (node);
}

t_node	*parse_line(t_token **rest, int *ret)
{
	t_node	*node;
	t_node	*rhs;

	node = parse_command(rest, ret);
	if (!node)
		return (NULL);
	while (consume_reserved(rest, "|"))
	{
		rhs = parse_command(rest, ret);
		if (!rhs)
		{
			free_ast(node);
			return (NULL);
		}
		node = new_pipe_node(node, rhs);
		if (!node)
		{
			*ret = -1;
			return (NULL);
		}
	}
	return (node);
}

int	parse(t_token *tokens, t_node **ast)
{
	t_token	*rest;
	int		ret;

	ret = 0;
	if (tokens->type == TK_EOF)
	{
		(*ast) = NULL;
		return (0);
	}
	rest = tokens;
	(*ast) = parse_line(&rest, &ret);
	return (ret);
}
