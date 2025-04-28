#include "minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
    void	*new;

    new = ft_calloc(new_size, 1);
    if (!new)
        return (NULL);
    if (ptr)
    {
        ft_memcpy(new, ptr, old_size);
        free(ptr);
    }
    return (new);
}

// トークンが予約語かつ指定された記号に一致するか確認して次のトークンに進める
int	consume(t_token **rest, t_token *token, char *op)
{
	if (token->type != TK_RESERVED || ft_strncmp(token->str, op, ft_strlen(op)) != 0)
		return (0);
	*rest = token->next;
	return (1);
}

// トークンがTK_WORDかどうか確認
int	peek_word(t_token *token)
{
	if (token->type == TK_WORD)
		return (1);
	return (0);
}

// トークンがリダイレクト演算子かどうか確認
int	peek_redir_op(t_token *token)
{
	if (token->type != TK_RESERVED)
		return (0);
	if (is_two_metachar(token->str) || is_single_metachar(token->str))
		return (1);
	return (0);
}

// トークンがTK_WORDかどうか確認して文字列を返し次のトークンに進める
char	*expect_word(t_token **rest, t_token *cur)
{
	if (!peek_word(cur))
	{
		ft_dprintf(STDERR_FILENO, "parse error: expected word, but got: %s\n",
			cur->str);
		return (NULL);
	}
	*rest = cur->next;
	return (cur->str);
}

// 新しいパイプノードを作成
t_node	*new_pipe_node(t_node *left, t_node *right)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->kind = ND_PIPE;
	node->left = left;
	node->right = right;
	return (node);
}

// 新しいコマンドノードを作成
t_node	*new_command_node(void)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->kind = ND_CMD;
	node->argv = NULL;
	node->argc = 0;
	node->redirs = NULL;
	node->redir_count = 0;
	return (node);
}

// トークンを解析してリダイレクトノードを生成
t_redir	*parse_redir(t_token **rest, t_token *cur)
{
	t_redir	*redir;

	redir = ft_calloc(1, sizeof(t_redir));
	if (consume(rest, cur, ">>"))
		redir->kind = REDIR_APPEND;
	else if (consume(rest, cur, ">"))
		redir->kind = REDIR_OUT;
	else if (consume(rest, cur, "<<"))
		redir->kind = REDIR_HEREDOC;
	else if (consume(rest, cur, "<"))
		redir->kind = REDIR_IN;
	else
	{
		ft_dprintf(STDERR_FILENO, "parse error: unexpected redirection token: %s\n",
			cur->str);
		return (NULL);
	}
	cur = *rest;
	redir->filename = expect_word(rest, cur);
    if (!redir->filename)
    {
        free(redir);
        return (NULL);
    }
	return (redir);
}

// トークンを解析してコマンドノード（パイプ以外の部分をまとめたノード）を生成
t_node	*parse_command(t_token **rest, t_token *cur)
{
	t_node	*node;

	node = new_command_node();
	if (!node)
		return (NULL);
    // TK_WORDがある間、argvに追加
	while (peek_word(cur))
	{
		node->argc++;
		node->argv = ft_realloc(node->argv, sizeof(char *) * node->argc, sizeof(char *) * (node->argc + 1));
		node->argv[node->argc - 1] = ft_strdup(cur->str);
		if (!node->argv[node->argc])
		{
			free(node->argv[node->argc]);
			free(node);
			return (NULL);
		}
		node->argv[node->argc] = NULL;
		cur = cur->next;
	}
    // リダイレクト演算子がある間、redirsに追加
	while (peek_redir_op(cur))
	{
		node->redir_count++;
		node->redirs = ft_realloc(node->redirs, sizeof(t_redir *)
        * node->redir_count, sizeof(t_redir *)
				* (node->redir_count + 1));
		node->redirs[node->redir_count - 1] = parse_redir(&cur, cur);
        if (!node->redirs[node->redir_count - 1])
        {
            free(node->redirs[node->redir_count - 1]);
            free(node->redirs);
            free(node);
            return (NULL);
        }
        node->redirs[node->redir_count] = NULL;
        cur = cur->next;
	}
	if (node->argc == 0 && node->redir_count == 0)
	{
		ft_dprintf(STDERR_FILENO, "parse error (parse_command)\n");
		free(node);
		return (NULL);
	}
	*rest = cur;
	return (node);
}

// / トークンを解析してASTを生成
t_node	*parse_line(t_token **rest, t_token *cur)
{
	t_node	*node;
	t_node	*right;

	node = parse_command(&cur, cur);
	if (!node)
		return (NULL);
	while (consume(&cur, cur, "|"))
	{
        // 右側のコマンドを解析
		right = parse_command(&cur, cur);
		if (!right)
			return (NULL);
        // パイプノードを作成し、それを根として左右のノードを接続
		node = new_pipe_node(node, right);
		if (!node)
			return (NULL);
	}
	*rest = cur;
	return (node);
}

// 構文解析
int	parse(t_token *tokens, t_node **ast)
{
	// トークンの残り（tokensをfreeできるように別変数で管理）
	t_token	*rest;

	// トークンの先頭をrestに設定
	rest = tokens;
	// トークンを解析してASTを生成
	*ast = parse_line(&rest, tokens);
	if (!(*ast))
		return (-1);
	// 残りのトークンがEOFでない場合はエラー
	if (rest->type != TK_EOF)
	{
		ft_dprintf(STDERR_FILENO,
					"parse error: extra token after valid input: \
			%s\n",
					rest->str);
		return (-1);
	}
	return (0);
}
