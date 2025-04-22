#include "minishell.h"

void	init(t_shell *shell)
{
	shell->status = 0;
	// TODO: 直前のexitステータスを扱うように変更
	shell->last_status = 0;
	shell->tokens = NULL;
}
