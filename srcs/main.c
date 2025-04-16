#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	**cmds;

	// TODO: あとで消す(かも)
	(void)argc;
	(void)argv;
	input = NULL;
	cmds = NULL;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		/* TODO: 入力行を解析 */
		// TODO: あとで消す
		// 仮パース
		cmds = ft_split(input, ' ');
		if (!cmds)
		{
			free(input);
			exit(EXIT_FAILURE);
		}
		execute(cmds, envp);
		// TODO: あとで消す
		free_2d_array((void **)cmds);
		free(input);
	}
	exit(EXIT_SUCCESS);
}
