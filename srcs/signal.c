#include "minishell.h"

// SIGINTのシグナルハンドラ
void	sigint_handler(int signum)
{
    g_sig_received = signum;
    rl_replace_line("", 0);
    rl_done = 1;
}

void    set_heredoc_sigint(void)
{
    struct sigaction sa;

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("minishell");
        exit(1);
    }
}

void    set_sigint(void)
{
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("minishell");
        exit(1);
    }
}

// SIGINTとSIGQUITに対してシグナルハンドラなどの設定
void	init_signals(void)
{
	struct sigaction	sa;

	set_sigint();
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		perror("minishell");
		exit(1);
	}
}
