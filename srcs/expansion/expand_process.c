#include "minishell.h"

// 展開しない & クォートのついてない文字列を抜粋してexpトークンを作成
// 合わせてポインタを進める
t_exp_tkn	*extract_literal(char **s, int len)
{
	char	*str;

	str = ft_strndup(*s, len);
	if (!str)
		return (NULL);
	*s += len;
	return (new_exp_token(str, false));
}

// ダブルクォート内の処理
int	handle_double_quote(t_exp_tkn **head, char **p)
{
	int			len;
	t_exp_tkn	*new;

	while (**p && **p != '"')
	{
		if (**p == '$')
		{
			new = expand_env_var(p);
			if (!new)
				return (1);
			append_exp_token(head, new);
		}
		else
		{
			len = 0;
			while ((*p)[len] && (*p)[len] != '"' && (*p)[len] != '$')
				len++;
			new = extract_literal(p, len);
			if (!new)
				return (1);
			append_exp_token(head, new);
		}
	}
	return (0);
}

// シングルクォート内の処理
int	handle_single_quote(t_exp_tkn **head, char **p)
{
	int			len;
	t_exp_tkn	*new;

	len = 0;
	while ((*p)[len] && (*p)[len] != '\'')
		len++;
	new = extract_literal(p, len);
	if (!new)
		return (1);
	append_exp_token(head, new);
	return (0);
}

// クォートなし部分の処理
int	handle_no_quote(t_exp_tkn **head, char **p)
{
	int			len;
	t_exp_tkn	*new;

	if (**p == '$')
	{
		new = expand_env_var(p);
		if (!new)
			return (1);
		append_exp_token(head, new);
	}
	else
	{
		len = 0;
		while ((*p)[len] && (*p)[len] != '$')
			len++;
		new = extract_literal(p, len);
		if (!new)
			return (1);
		append_exp_token(head, new);
	}
	return (0);
}

// 変数展開とクォート除去
int	tokenize_with_expansion(t_exp_tkn **head, char *str)
{
	char	*p;
	int		res;

	p = str;
	while (*p)
	{
		if (*p == '"')
		{
			p++;
			res = handle_double_quote(head, &p);
			p++;
		}
		else if (*p == '\'')
		{
			p++;
			res = handle_single_quote(head, &p);
			p++;
		}
		else
			res = handle_no_quote(head, &p);
		if (res != 0)
			return (1);
	}
	return (0);
}
