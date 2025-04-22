#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	shell;

	// TODO: (void)あとで消す(かも)
	(void)argc;
	(void)argv;
    (void)envp;
	input = NULL;
	init(&shell);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		// TODO: tokenize関数もステータスとトークンをセットするように。
        shell.tokens = tokenize(input);
        if (!shell.tokens)
        {
            free(input);
            exit(EXIT_FAILURE);
        }
		expand_tokens(shell.tokens, &shell.status);
        /* TODO: 入力行を解析 */
        // TODO: debugあとで消す
        debug_tokenizer(shell.tokens);
		// execute(tokens, envp);
		free(input);
	}
	exit(EXIT_SUCCESS);
}
