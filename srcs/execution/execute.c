#include "minishell.h"

int	execute(char **cmds, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell");
		free_2d_array((void **)cmds);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
		execute_in_child(cmds, envp);
	waitpid(pid, &status, 0);
	return (status);
}
