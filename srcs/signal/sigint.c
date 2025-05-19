#include "minishell.h"

// SIGINTのシグナルハンドラ
void	handle_sigint(int signum)
{
	g_sig_received = signum;
	rl_replace_line("", 0);
	rl_done = 1;
}

void	handle_heredoc_sigint(int signum)
{
	g_sig_received = signum;
	write(STDOUT_FILENO, "\n", 1);
}

// コマンド実行時のSIGINT設定
void	set_exec_sigint(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("minishell");
		exit(1);
	}
}

// ヒアドキュメント時のSIGINT設定
void	set_heredoc_sigint(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_heredoc_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("minishell");
		exit(1);
	}
}

// メインプロセスのSIGINT設定
void	set_main_sigint(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handle_sigint;
	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("minishell");
		exit(1);
	}
}
