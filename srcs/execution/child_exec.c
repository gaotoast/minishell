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
				perror("minishell");
				free_2d_array((void **)cmds);
				exit(1);
			}
		}
		perror("minishell");
		exit(EXIT_FAILURE);
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
			perror("minishell");
			free_2d_array((void **)cmds);
			exit(EXIT_FAILURE);
		}
	}
	perror("minishell");
	exit(EXIT_FAILURE);
}

// 子プロセス内でのコマンド実行
void	execute_in_child(char **cmds, char **envp)
{
	char	*path_env;
	char	*cmd_path;

	exec_if_relative_path(cmds, envp);
	path_env = ft_getenv("PATH", envp);
	if (!path_env || ft_strncmp(cmds[0], "/", 1) == 0)
		exec_if_absolute_path(cmds, envp);
	cmd_path = resolve_cmd_path(cmds[0], path_env);
	if (!cmd_path)
	{
		free_2d_array((void **)cmds);
		exit(EXIT_FAILURE);
	}
	execve(cmd_path, cmds, envp);
	perror("minishell");
	free_2d_array((void **)cmds);
	exit(EXIT_FAILURE);
}
