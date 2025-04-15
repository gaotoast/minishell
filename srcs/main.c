#include "minishell.h"

int	main(void)
{
	char	*input;

	input = NULL;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input) // 空の文字列でなければ履歴に追加
			add_history(input);
		/* TODO: 入力行を実行 */
		free(input);
	}
	exit(0);
}
