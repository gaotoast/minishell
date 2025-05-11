#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	*shell;

	// TODO: (void)あとで消す(かも)
	(void)argc;
	(void)argv;
	input = NULL;
	if (init(&shell, envp) < 0)
		exit(EXIT_FAILURE);
    rl_clear_history();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		tokenize(input, &shell->tokens);
        // debug_tokenizer(shell->tokens);
        parse(shell->tokens, &shell->ast);
		expand(shell->ast, shell->envp_cp);
        // debug_expand(shell->ast);
		execute(shell->ast, envp);
		// 毎ループ更新されるためfree
		free(input);
		free_tokens(shell->tokens);
		shell->tokens = NULL;
		free_ast(shell->ast);
		shell->ast = NULL;
	}
	free_shell(shell);
	exit(EXIT_SUCCESS);
}
