/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stakada <stakada@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:23:59 by stakada           #+#    #+#             */
/*   Updated: 2025/07/15 20:22:47 by stakada          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin_cmd(t_node *node)
{
	char	*cmd;
	int		stat;

	stat = 0;
	cmd = node->argv[0];
	if (ft_strncmp(cmd, "echo", 5) == 0)
		stat = echo(node->argc, node->argv_lst);
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		stat = cd(node->argc, node->argv);
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		stat = pwd(node->argc, node->argv);
	else if (ft_strncmp(cmd, "export", 7) == 0)
		stat = export(node->argc, node->argv);
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		stat = unset(node->argc, node->argv);
	else if (ft_strncmp(cmd, "env", 4) == 0)
		stat = env(node->argc, node->argv);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		stat = ft_exit(node->argc, node->argv, 1);
	return (sh_stat(ST_SET, stat));
}

int	restore_std_fds(int stashed_stdin, int stashed_stdout)
{
	if (stashed_stdin != -1)
	{
		if (dup2(stashed_stdin, STDIN_FILENO) == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: dup2: %s\n", strerror(errno));
			return (-1);
		}
		close(stashed_stdin);
	}
	if (stashed_stdin != -1)
	{
		if (dup2(stashed_stdout, STDOUT_FILENO) == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: dup2: %s\n", strerror(errno));
			return (-1);
		}
		close(stashed_stdout);
	}
	return (0);
}

static int	backup_std_fds(char *cmd, int *stashed_stdin, int *stashed_stdout)
{
	if (ft_strncmp(cmd, "exit", 5) != 0)
	{
		*stashed_stdin = dup(STDIN_FILENO);
		*stashed_stdout = dup(STDOUT_FILENO);
		if (*stashed_stdin == -1 || *stashed_stdout == -1)
		{
			ft_dprintf(STDERR_FILENO, "minishell: dup: %s\n", strerror(errno));
			return (-1);
		}
		return (0);
	}
	return (0);
}

int	process_builtin_direct(t_node *node)
{
	int	stashed_stdin;
	int	stashed_stdout;
	int	ret;

	ret = 0;
	stashed_stdin = -1;
	stashed_stdout = -1;
	if (backup_std_fds(node->argv[0], &stashed_stdin, &stashed_stdout) != 0)
		return (-1);
	ret = apply_redirs(node->redir_count, node->redirs);
	if (ret != 0)
	{
		restore_std_fds(stashed_stdin, stashed_stdout);
		unlink_all_temp(node);
		return (ret);
	}
	exec_builtin_cmd(node);
	unlink_all_temp(node);
	if (restore_std_fds(stashed_stdin, stashed_stdout) != 0)
		return (-1);
	return (0);
}
