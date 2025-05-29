#include "minishell.h"

// 環境変数名に含むことができる文字かどうか判定
static int	is_valid_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

// 変数名部分を抜粋して引数のnameに設定（例：$HOME@# -> HOME）
static int	extract_var_name(char *str, char **name)
{
	int	len;

	if (*str == '?')
	{
		*name = ft_strdup("?");
		if (!(*name))
			return (-1);
		return (0);
	}
	len = 0;
	while (str[len] && is_valid_var_char(str[len]))
		len++;
	if ((!ft_isalpha(str[0]) && str[0] != '_') || len == 0)
	{
		*name = NULL;
		return (0);
	}
	*name = ft_strndup(str, len);
	if (!(*name))
		return (-1);
	return (0);
}

static char	*get_var_value(char *name)
{
	char	*value;
	char	*env_value;

	if (ft_strncmp(name, "?", 2) == 0)
		value = ft_itoa(sh_stat(ST_GET, 0));
	else
	{
		env_value = ft_env(ENV_GET_VAL, name);
		if (!env_value)
			value = ft_strdup("");
		else
			value = ft_strdup(env_value);
	}
	return (value);
}

// 環境変数または$?を展開してexpトークンを作成
t_exp_tkn	*expand_env_var(char **s, bool is_quoted)
{
	char	*name;
	char	*value;

	(*s)++;
	if (extract_var_name(*s, &name) < 0)
		return (NULL);
	if (!name)
	{
		value = ft_strdup("$");
		if (!value)
			return (NULL);
		return (new_exp_token(value, false, is_quoted));
	}
	*s += ft_strlen(name);
	value = get_var_value(name);
	free(name);
	if (!value)
		return (NULL);
	return (new_exp_token(value, true, is_quoted));
}
