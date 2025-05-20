#include "minishell.h"

// パイプラインの実行
pid_t	run_pipeline(t_node *node, char **envp, int count)
{
	pid_t	pid;

	if (!node)
		return (-1);
	prepare_pipe(node);
	pid = fork();
	if (pid < 0)
	{
		perror("minishell");
		return (-1);
	}
	if (pid == 0)
	{
		prepare_pipe_child(node, count);
		child_exec(node, envp);
	}
	prepare_pipe_parent(node, count);
	if (node->next_cmd)
		return (run_pipeline(node->next_cmd, envp, count + 1));
	return (pid);
}

// 実行部メイン処理
void	execute(t_node *root, char **envp)
{
	pid_t	last_pid;
	t_node	*first_cmd;
	t_node	*last_cmd;

	if (!root)
		return ;
	first_cmd = NULL;
	last_cmd = NULL;
	// ビルトインコマンド単体の場合直接実行
	if (root->kind == ND_CMD && root->argv && is_builtin(root->argv[0]))
	{
		process_builtin_direct(root, envp);
		return ;
	}
	link_exec_nodes(root, root->rhs, &first_cmd, &last_cmd);
	last_pid = run_pipeline(first_cmd, envp, 0);
	sh_stat(ST_SET, wait_children(last_pid));
}
