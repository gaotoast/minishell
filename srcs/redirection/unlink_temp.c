#include "minishell.h"

// ヒアドキュメントの一時ファイルをすべて削除
void	unlink_all_temp(int redir_count, t_redir **redirs)
{
	int	i;

	i = 0;
	while (i < redir_count)
	{
		if (redirs[i]->temp_file)
		{
			unlink(redirs[i]->temp_file);
			free(redirs[i]->temp_file);
		}
		i++;
	}
}
