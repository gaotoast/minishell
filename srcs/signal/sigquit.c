#include "minishell.h"

// SIGQUITシグナルハンドラ
void	handle_sigquit(int signum)
{
	g_sig_received = signum;
}

// コマンド実行時のSIGQUIT設定
void	set_exec_sigquit(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handle_sigquit;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("minishell");
		exit(1);
	}
}

// メインプロセスのSIGQUIT設定
void	set_main_sigquit(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("minishell");
		exit(1);
	}
}
