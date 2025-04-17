#include "minishell.h"

// パイプとリダイレクトの記号を判定
int is_single_metachar(char *p)
{
    if (*p == '|' || *p == '<' || *p == '>')
        return (1);
    return (0);
}

// heredocとappendの記号を判定
int is_two_metachar(char *p)
{
    if (ft_strncmp(p, "<<", 2) == 0 || ft_strncmp(p, ">>", 2) == 0)
        return (1);
    return (0);
}

// // 引用符を判定
// int is_quote(char *p)
// {
//     if (*p == '\'' || *p == '"')
//         return (1);
//     return (0);
// }

int ft_isspace(char c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
        return (1);
    return (0);
}

// トークンを追加（新しいトークンへのポインタを返すことでcurrentを一つ進める）
t_token *add_token(t_token *cur, t_token_type type, char *start, int len)
{
    t_token *new;

    new = (t_token *)malloc(sizeof(t_token));
    if (!new)
    {
        perror("minishell");
        return (NULL);
    }
    new->str = (char *)ft_calloc(sizeof(char), len + 1);
    if (!new->str)
    {
        perror("minishell");
        free(new);
        return (NULL);
    }
    ft_memcpy(new->str, (void *)start, len);
    new->type = type;
    new->next = NULL;
    cur->next = new;
    return (new);
}

// 入力をトークンに分割
t_token *torknizer(char *line)
{
    t_token head;
    t_token *cur;
    char *p;
    char *start;

    head.next = NULL;
    cur = &head;
    p = line;
    while (*p)
    {
        // 空白文字をスキップ
        if (ft_isspace(*p))
            p++;
        // 2文字のメタ文字をトークン化
        else if (is_two_metachar(p))
        {
            cur = add_token(cur, TK_RESERVED, p, 2);
            if (!cur)
                return (NULL);
            p += 2;
        }
        // 1文字のメタ文字をトークン化
        else if (is_single_metachar(p))
        {
            cur = add_token(cur, TK_RESERVED, p, 1);
            if (!cur)
                return (NULL);
            p++;
        }
        // 単語をトークン化
        else
        {
            start = p;
            while (*p && !ft_isspace(*p) && !is_single_metachar(p) && !is_two_metachar(p))
                p++;
            cur = add_token(cur, TK_WORD, start, p - start);
            if (!cur)
                return (NULL);
        }
    }
    // EOFトークンを追加
    if (!add_token(cur, TK_EOF, p, 0))
        return (NULL);
    return (head.next);
}
