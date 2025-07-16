#include "minishell.h"

int	is_empty_cmds(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (argv[i][0] != '\0')
			return (0);
		i++;
	}
	return (1);
}

// リソース解放後、子プロセスをexit
void	child_exit(char **env, t_node *node, int exit_stat)
{
	free(env);
	unlink_all_temp(node);
	inner_exit(exit_stat);
}

// 子プロセス内での実行
void	child_exec(t_node *node)
{
	char	**cp_env;

	cp_env = (char **)ft_env(ENV_GET_ALL_EX, NULL);
	if (!cp_env)
		inner_exit(1);
	if (apply_redirs(node->redir_count, node->redirs) != 0)
		child_exit(cp_env, node, 1);
	if (is_empty_cmds(node->argv))
	{
		ft_dprintf(STDERR_FILENO, "%s: command not found\n", node->argv[0]);
		child_exit(cp_env, node, 127);
	}
	if (is_builtin(node->argv[0]))
	{
		exec_builtin_cmd(node);
		child_exit(cp_env, node, sh_stat(ST_GET, 0));
	}
	else
		exec_cmd(node->argv, cp_env);
}

// パイプラインの実行
pid_t	run_pipeline(t_node *node, int count)
{
	pid_t	pid;

	if (!node)
		return (-1);
	if (prepare_pipe(node) != 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		ft_dprintf(STDERR_FILENO, "minishell: fork: %s\n", strerror(errno));
		return (-1);
	}
	if (pid == 0)
	{
		prepare_pipe_child(node, count);
		child_exec(node);
	}
	prepare_pipe_parent(node, count);
	if (node->next_cmd)
		return (run_pipeline(node->next_cmd, count + 1));
	return (pid);
}

// 実行部メイン処理
// ASTのコマンドノードをリストとしてつなげる -> ヒアドキュメントの入力を処理 -> ビルトイン単体かその他かで分岐して実行
int	execute(t_node *root)
{
	pid_t	last_pid;
	t_node	*first_cmd;
	t_node	*last_cmd;
	int		ret;

	ret = 0;
	if (!root)
		return (0);
	first_cmd = NULL;
	last_cmd = NULL;
	link_exec_nodes(root, root->rhs, &first_cmd, &last_cmd);
	if (handle_all_heredocs(first_cmd) != 0)
		return (-1);
	// ビルトインコマンド単体の場合
	if (root->kind == ND_CMD && root->argv && is_builtin(root->argv[0]))
	{
		ret = process_builtin_direct(root);
		return (ret);
	}
	last_pid = run_pipeline(first_cmd, 0);
	if (last_pid < 0)
		return (-1);
	sh_stat(ST_SET, wait_children(last_pid, first_cmd));
	return (0);
}
