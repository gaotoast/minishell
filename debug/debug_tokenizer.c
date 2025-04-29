#include "minishell.h"

void	print_tokens(t_token *token)
{
	const char *type_str[] = {
		[TK_WORD] = "WORD",
		[TK_RESERVED] = "RESERVED",
		[TK_EOF] = "EOF"
	};

	while (token)
	{
		printf("[%-8s] %s\n", type_str[token->type], token->str);
		token = token->next;
	}
}

void	debug_tokenizer(t_token *tokens)
{
    if (!tokens)
    {
        printf("No tokens.\n");
        return ;
    }
    printf("===TOKENS===\n");
    print_tokens(tokens);
}
