#include "minishell.h"

char	**copy_env(char **envp)
{
	int		i;
	int		len;
	char	**rtn;

	len = 0;
	while (envp[len])
		++len;
	rtn = (char **)malloc(sizeof(char *) * (len + 1));
	i = 0;
	while (envp[i])
	{
		rtn[i] = ft_strdup(envp[i]);
		++i;
	}
	rtn[i] = NULL;
	return (rtn);
}

int main(int argc, char **argv)
{
	sh_stat(ST_SET, 42);
	ft_exit(argc, argv);
}

// int	main(void)
// {
// 	char	*input;

// 	input = NULL;
// 	while (1)
// 	{
// 		input = readline("minishell$ ");
// 		if (!input)
// 			break ;
// 		if (*input) // 空の文字列でなければ履歴に追加
// 			add_history(input);
// 		/* TODO: 入力行を実行 */
// 		free(input);
// 	}
// 	exit(0);
// }
