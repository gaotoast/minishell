#include "minishell.h"

// SIGINTとSIGQUITに対してシグナルハンドラなどの設定
void	init_signals(void)
{
	set_main_sigint();
	set_main_sigquit();
}
