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

// 変数展開とクォート除去
int	tokenize_with_expansion(t_exp_tkn **head, char *str, char **envp)
{
	char		*ptr;
	int			len;
	char		quote;
	t_exp_tkn	*new;

	ptr = str;
	while (*ptr)
	{
		len = 0;
		if (*ptr == '\'' || *ptr == '"')
		{
			quote = *ptr;
			ptr++;
			while (*ptr && *ptr != quote)
			{
				if (quote == '"' && *ptr == '$')
				{
					new = expand_env_var(&ptr, envp);
					if (!new)
						return (1);
					append_exp_token(head, new);
				}
				else
				{
					while (ptr[len] && ptr[len] != quote)
						len++;
					new = extract_literal(&ptr, len);
					if (!new)
						return (1);
					append_exp_token(head, new);
				}
			}
			ptr++;
		}
		else if (*ptr == '$')
		{
			new = expand_env_var(&ptr, envp);
			if (!new)
				return (1);
			append_exp_token(head, new);
		}
		else
		{
			while (ptr[len] && ptr[len] != '\'' && ptr[len] != '"'
				&& ptr[len] != '$')
				len++;
			new = extract_literal(&ptr, len);
			if (!new)
				return (1);
			append_exp_token(head, new);
		}
	}
	return (0);
}
