#include "minishell.h"

// exitステータス管理
int	sh_stat(int val, t_st_op op)
{
	static int	status;

	if (op == ST_SET)
		status = val;
	else if (op == ST_GET)
		return (status);
	return (status);
}
