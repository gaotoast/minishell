#include "minishell.h"

// 単一文字を結果文字列に追加
static char	*append_char(char *result, char c)
{
	char	*temp;

	temp = (char *)ft_malloc(ft_strlen(result) + 2);
	if (!temp)
	{
		free(result);
		return (NULL);
	}
	ft_strlcpy(temp, result, ft_strlen(result) + 1);
	temp[ft_strlen(result)] = c;
	temp[ft_strlen(result) + 1] = '\0';
	free(result);
	return (temp);
}

// 変数名が無い場合の$処理
static char	*handle_empty_name(char *result)
{
    char	*temp;

    temp = ft_strjoin(result, "$");
    free(result);
    return (temp);
}

// 変数値の取得と結合
static char	*join_var_value(char *result, char *name)
{
    char	*value;
    char	*temp;

    value = get_var_value(name);
    free(name);
    if (!value)
        return (result);
    temp = ft_strjoin(result, value);
    free(result);
    free(value);
    return (temp);
}

// $記号の処理
static char	*process_dollar(char **s, char *result)
{
	char	*name;

	(*s)++;
	if (extract_var_name(*s, &name) < 0)
	{
		free(result);
		return (NULL);
	}
	if (!name)
        return (handle_empty_name(result));
	*s += ft_strlen(name);
	return (join_var_value(result, name));
}

// ヒアドキュメント用の変数展開関数
int	expand_heredoc_line(char **line)
{
	char	*result;
	char	*s;

	result = ft_strdup("");
	if (!result)
		return (1);
	s = *line;
	while (*s)
	{
		if (*s == '$')
			result = process_dollar(&s, result);
		else
		{
			result = append_char(result, *s);
			s++;
		}
        if (!result)
            return (1);
	}
	free(*line);
	*line = result;
	return (0);
}
