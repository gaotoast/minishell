/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:23:51 by stakada           #+#    #+#             */
/*   Updated: 2025/07/16 16:07:41 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	error_and_exit(char *cmd, int exit_code, char **envp)
{
	ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmd, strerror(errno));
	free(envp);
	inner_exit(exit_code);
}

void	exec_if_relative_path(char **cmds, char **envp)
{
	struct stat	statbuf;

	if (ft_strncmp(cmds[0], "./", 2) == 0 || ft_strncmp(cmds[0], "../", 3) == 0)
	{
		if (access(cmds[0], F_OK) != 0)
			error_and_exit(cmds[0], 127, envp);
		if (stat(cmds[0], &statbuf) != 0)
			error_and_exit(cmds[0], 126, envp);
		if (!S_ISREG(statbuf.st_mode) || access(cmds[0], X_OK) != 0)
		{
			if (S_ISDIR(statbuf.st_mode))
				ft_dprintf(STDERR_FILENO, "minishell: %s: Is a directory\n",
					cmds[0]);
			else
				ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0],
					strerror(errno));
			free(envp);
			inner_exit(126);
		}
		execve(cmds[0], cmds, envp);
		error_and_exit(cmds[0], 1, envp);
	}
}

void	exec_if_absolute_path(char **cmds, char **envp)
{
	struct stat	statbuf;

	if (access(cmds[0], F_OK) != 0)
		error_and_exit(cmds[0], 127, envp);
	if (stat(cmds[0], &statbuf) != 0)
		error_and_exit(cmds[0], 126, envp);
	if (!S_ISREG(statbuf.st_mode) || access(cmds[0], X_OK) != 0)
	{
		if (S_ISDIR(statbuf.st_mode))
			ft_dprintf(STDERR_FILENO, "minishell: %s: Is a directory\n",
				cmds[0]);
		else
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n", cmds[0],
				strerror(errno));
		free(envp);
		inner_exit(126);
	}
	execve(cmds[0], cmds, envp);
	error_and_exit(cmds[0], 1, envp);
}

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
