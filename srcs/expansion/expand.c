#include "minishell.h"

char	*ft_strndup(char *str, int len)
{
	char	*result;

	if (!str || len <= 0)
		return (NULL);
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
	{
		perror("minishell");
		return (NULL);
	}
	result[0] = '\0';
	ft_strlcpy(result, str, len + 1);
	return (result);
}

void	append_string(char **result, char *str, int *status)
{
	size_t	result_len;
	size_t	str_len;
	char	*new_result;

	if (!result || !str)
	{
		(*status) = -1;
		return ;
	}
	if (!(*result))
		*result = ft_strdup(str);
	if (!(*str))
		return ;
	result_len = ft_strlen(*result);
	str_len = ft_strlen(str);
	new_result = (char *)malloc(result_len + str_len + 1);
	if (!new_result)
	{
		(*status) = -1;
		return ;
	}
	ft_strlcpy(new_result, *result, result_len + 1);
	ft_strlcat(new_result, str, result_len + str_len + 1);
	free(*result);
	*result = new_result;
}

void	append_char(char **result, char c, int *status)
{
	size_t	len;
	char	*new_result;

	if (!result)
	{
		(*status) = -1;
		return ;
	}
	if (!(*result))
		*result = ft_strdup(&c);
	len = ft_strlen(*result);
	new_result = (char *)malloc(len + 2);
	if (!new_result)
	{
		*status = -1;
		return ;
	}
	ft_strlcpy(new_result, *result, len + 1);
	new_result[len] = c;
	new_result[len + 1] = '\0';
	free(*result);
	*result = new_result;
}

int	is_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	return (0);
}

char	*get_var_name(char *str)
{
	int	len;

	if (*str == '?')
		return (ft_strdup("?"));
	len = 0;
	while (str[len] && is_var_char(str[len]))
		len++;
	if (len == 0)
		return (NULL);
	return (ft_strndup(str, len));
}

char	*expand_var(char **s)
{
	char	*name;
	char	*value;
	char	*env;

	(*s)++;
	name = get_var_name(*s);
	if (!name)
		return (ft_strdup("$"));
	*s += ft_strlen(name);
	if (ft_strncmp(name, "?", 2) == 0)
		value = ft_itoa(0); // TODO: 実際のexitステータスに変更
	else
	{
		env = getenv(name);
		if (!env)
			value = ft_strdup("");
		else
			value = ft_strdup(env);
	}
	if (!value)
		value = ft_strdup("");
	free(name);
	return (value);
}

char	*process_quotes(char *str, int *status)
{
	char	*result;
	char	*ptr;
	char	quote;

	result = ft_strdup("");
	ptr = str;
	while (*ptr)
	{
		if (*ptr == '\'' || *ptr == '"')
		{
			quote = *ptr;
			ptr++;
			while (*ptr && *ptr != quote)
			{
				if (quote == '"' && *ptr == '$')
					append_string(&result, expand_var(&ptr), status);
				else
				{
					append_char(&result, *ptr, status);
					ptr++;
				}
			}
			ptr++;
		}
		else if (*ptr == '$')
			append_string(&result, expand_var(&ptr), status);
		else
		{
			append_char(&result, *ptr, status);
			ptr++;
		}
	}
	return (result);
}

void	expand_tokens(t_token *tokens, int *status)
{
	t_token	*cur;
	char	*expanded;

	cur = tokens;
	while (cur)
	{
		if (cur->type == TK_WORD)
		{
			expanded = process_quotes(cur->str, status);
			if (!expanded)
			{
				(*status) = -1;
				return ;
			}
			free(cur->str);
			cur->str = expanded;
		}
		cur = cur->next;
	}
}
