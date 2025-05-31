#include "minishell.h"

// exitステータス管理
int	sh_stat(t_st_op op, int val)
{
	static int	status;
	int			ret;

	if (op == ST_SET)
		status = val;
	else if (op == ST_GET)
	{
		if (g_sig_received)
		{
			ret = g_sig_received + 128;
			g_sig_received = 0;
			return (ret);
		}
		else
		{
			ret = status;
			status = 0;
			return (ret);
		}
	}
	return (status);
}
