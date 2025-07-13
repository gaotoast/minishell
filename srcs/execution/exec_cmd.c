#include "minishell.h"

// 相対パスなら実行
void	exec_if_relative_path(char **cmds, char **envp)
{
	if (ft_strncmp(cmds[0], "./", 2) == 0 || ft_strncmp(cmds[0], "../", 3) == 0)
	{
		if (access(cmds[0], F_OK) == 0)
		{
			if (access(cmds[0], X_OK) == 0)
			{
				execve(cmds[0], cmds, envp);
				ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0],
					strerror(errno));
				free(envp);
				inner_exit(1);
			}
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0],
				strerror(errno));
			free(envp);
			inner_exit(126);
		}
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0],
			strerror(errno));
		free(envp);
		inner_exit(127);
	}
}

// 絶対パスなら実行
void	exec_if_absolute_path(char **cmds, char **envp)
{
	if (access(cmds[0], F_OK) == 0)
	{
		if (access(cmds[0], X_OK) == 0)
		{
			execve(cmds[0], cmds, envp);
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0],
				strerror(errno));
			free(envp);
			inner_exit(1);
		}
		ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0],
			strerror(errno));
		free(envp);
		inner_exit(126);
	}
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0], strerror(errno));
	free(envp);
	inner_exit(127);
}

// コマンド実行
void	exec_cmd(char **cmds, char **envp)
{
	char	*path_env;
	char	*cmd_path;
	int		status;

	status = 1;
	exec_if_relative_path(cmds, envp);
	path_env = ft_env(ENV_GET_VAL, "PATH");
	if (!path_env || ft_strncmp(cmds[0], "/", 1) == 0)
		exec_if_absolute_path(cmds, envp);
	cmd_path = resolve_cmd_path(cmds[0], path_env, &status);
	if (!cmd_path)
	{
		free(envp);
		inner_exit(status);
	}
	set_exec_sigint();
	set_exec_sigquit();
	execve(cmd_path, cmds, envp);
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0], strerror(errno));
	free(envp);
	free(cmd_path);
	inner_exit(1);
}
