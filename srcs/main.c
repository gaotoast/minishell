#include "minishell.h"

volatile sig_atomic_t    g_sig_received;

int event(void)
{
    return (0);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	*shell;

	// TODO: (void)あとで消す(かも)
	(void)argc;
	(void)argv;
    rl_event_hook = event;
    g_sig_received = 0;
	input = NULL;
	if (init(&shell, envp) < 0)
        exit(1);
    rl_clear_history();
	while (1)
	{
        init_signals();
		input = readline("minishell$ ");
		if (!input)
        {
            write(STDOUT_FILENO, "exit\n", 5);
			break ;
        }
		if (*input)
			add_history(input);
        // TODO: それぞれエラー時の処理を追加
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
	exit(0);
}
