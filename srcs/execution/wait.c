#include "minishell.h"

int	wait_children(pid_t last_pid)
{
	pid_t	wait_ret;
	int		w_status;
	int		status;

	while (1)
	{
		wait_ret = wait(&w_status);
		if (wait_ret == last_pid)
			status = WEXITSTATUS(w_status);
		else if (wait_ret < 0)
		{
			if (errno == ECHILD)
				break ;
		}
	}
	return (status);
}
