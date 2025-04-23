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
		// TODO: exitするかどうか & ステータスの管理
		shell.status = tokenize(input, &(shell.tokens));
		if (shell.status < 0)
			exit(EXIT_FAILURE);
		shell.status = expand_tokens(shell.tokens);
		if (shell.status < 0)
			exit(EXIT_FAILURE);
		/* TODO: 入力行を解析 */
		// TODO: debugあとで消す
		debug_tokenizer(shell.tokens);
		// execute(tokens, envp);
		free(input);
	}
	exit(EXIT_SUCCESS);
}
