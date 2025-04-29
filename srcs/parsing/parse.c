#include "minishell.h"

// トークンがTK_RESERVEDかつ指定された記号に一致するか確認して次のトークンに進める
int	consume(t_token **rest, t_token *token, char *op)
{
	// TK_RESERVEDではない、または、opと異なっていればエラー
	if (token->type != TK_RESERVED || ft_strncmp(token->str, op, ft_strlen(op)
			+ 1) != 0)
		return (0);
	// トークンの残りを一つ消費
	*rest = token->next;
	return (1);
}

// トークンがTK_WORDかどうか確認
int	peek_word(t_token *token)
{
	// トークンがTK_WORDかどうか
	if (token->type == TK_WORD)
		return (1);
	return (0);
}

// トークンがリダイレクト演算子かどうか確認
int	peek_redir_op(t_token *token)
{
	// トークンが予約語でなければエラー
	if (token->type != TK_RESERVED)
		return (0);
	// トークンがリダイレクト記号かどうか
	if (ft_strncmp(token->str, ">>", 3) == 0 || ft_strncmp(token->str, ">",
			2) == 0 || ft_strncmp(token->str, "<<", 3) == 0
		|| ft_strncmp(token->str, "<", 2) == 0)
		return (1);
	return (0);
}

// トークンがTK_WORDかどうか確認して文字列を返し次のトークンに進める
int	expect_word(t_token **rest, t_token *cur, char **str)
{
	// トークンがTK_WORDでなければエラー
	if (!peek_word(cur))
	{
		ft_dprintf(STDERR_FILENO, "parse error: expected word, but got: %s\n",
			cur->str);
		(*str) = NULL;
		return (0);
	}
	// トークンの残りを一つ消費
	*rest = cur->next;
	// トークンの文字列をコピー
	(*str) = ft_strdup(cur->str);
	if (!(*str))
	{
		(*str) = NULL;
		return (0);
	}
	return (1);
}

// 新しいパイプノードを作成して左右のノードを接続
t_node	*new_pipe_node(t_node *lhs, t_node *rhs)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->kind = ND_PIPE;
	node->lhs = lhs;
	node->rhs = rhs;
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
	if (!redir)
		return (NULL);
	if (consume(rest, cur, ">>"))
		redir->kind = REDIR_APPEND;
	else if (consume(rest, cur, ">"))
		redir->kind = REDIR_OUT;
	else if (consume(rest, cur, "<<"))
		redir->kind = REDIR_HEREDOC;
	else if (consume(rest, cur, "<"))
		redir->kind = REDIR_IN;
	// リダイレクト記号以外ならエラー
	else
	{
		ft_dprintf(STDERR_FILENO,
					"minishell: syntax error near unexpected token \
			`%s\n'",
					cur->str);
		free(redir);
		return (NULL);
	}
	// 判定するトークンをリダイレクト記号の次のトークンに進める
	cur = *rest;
	// リダイレクト記号の次がTK_WORD以外ならエラー
	if (!expect_word(rest, cur, &redir->str))
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `newline'");
		free(redir);
		return (NULL);
	}
	return (redir);
}

// トークンを解析してコマンドノード（パイプ以外の部分をまとめたノード）を生成
t_node	*parse_command(t_token **rest, t_token *cur)
{
	t_node	*node;
	char	**argv_tmp;
	t_redir	**redirs_tmp;
	int		i;

	node = new_command_node();
	if (!node)
		return (NULL);
	// TK_WORDがある間、argvに追加
	while (peek_word(cur))
	{
		node->argc++;
		argv_tmp = (char **)malloc(sizeof(char *) * (node->argc + 1));
		if (!argv_tmp)
		{
			perror("minishell");
			free(node);
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
			free_2d_array(argv_tmp);
			return (NULL);
		}
		argv_tmp[node->argc] = NULL;
		free(node->argv);
		node->argv = argv_tmp;
		cur = cur->next;
	}
	// リダイレクト演算子がある間、リダイレクト記号と次のTK_WORD（ファイル名かheredocのデリミタ）をredirsに追加
	while (peek_redir_op(cur))
	{
		node->redir_count++;
		redirs_tmp = (t_redir **)malloc(sizeof(t_redir *) * (node->redir_count
					+ 1));
		if (!redirs_tmp)
		{
			perror("minishell");
			free(node);
			return (NULL);
		}
		i = 0;
		while (i < node->redir_count - 1)
		{
			redirs_tmp[i] = node->redirs[i];
			i++;
		}
		redirs_tmp[node->redir_count - 1] = parse_redir(&cur, cur);
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
	// コマンドが空でリダイレクトもない場合はエラー
	if (node->argc == 0 && node->redir_count == 0)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n");
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
	t_node	*rhs;

	// 最初のパイプまでを解析
	node = parse_command(&cur, cur);
	if (!node)
	{
		printf("invalid node\n");
		return (NULL);
	}
	while (consume(&cur, cur, "|"))
	{
		// 右側のコマンドを解析
		rhs = parse_command(&cur, cur);
		if (!rhs)
			return (NULL);
		// パイプノードを作成し、それを根として左右のノードを接続
		node = new_pipe_node(node, rhs);
		if (!node)
			return (NULL);
	}
	*rest = cur;
	return (node);
}

// 構文解析
int	parse(t_token *tokens, t_node **ast)
{
	// トークンの残り（引数のtokensをfreeできるように別変数で管理）
	t_token	*rest;

    // トークンがTK_EOFなら何もしない
	if (tokens->type == TK_EOF)
		return (0);
	// トークンの先頭をrestに設定
	rest = tokens;
	// トークンを解析してASTを生成
	(*ast) = parse_line(&rest, rest);
	if (!(*ast))
		return (-1);
	// 残りのトークンがEOFでない場合はエラー
	if (rest->type != TK_EOF)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `%s'\n", rest->str);
		return (-1);
	}
	return (0);
}
