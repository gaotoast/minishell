#include "minishell.h"

// TODO: pipe()とdup2()のエラーハンドリング

// pipe()でパイプを作る
void	prepare_pipe(t_node *node)
{
	if (node->next_cmd)
	{
		if (pipe(node->pipefd) < 0)
		{
			perror("minishell");
			// TODO: free()
			exit(1);
		}
	}
}

// 子プロセスのファイルディスクリプタ処理
void	prepare_pipe_child(t_node *node, int count)
{
	// 最初のコマンド以外：前のパイプの読み取り側をSTDINに
	if (count > 0)
	{
		dup2(node->in_fd, STDIN_FILENO);
		close(node->in_fd);
	}
	// 最後のコマンド以外：パイプの書き込み側をSTDOUTに & パイプの読み取り側（使わない）を閉じる
	if (node->next_cmd)
	{
		dup2(node->pipefd[1], STDOUT_FILENO);
		close(node->pipefd[1]);
		close(node->pipefd[0]);
	}
}

// 親プロセスのファイルディスクリプタ処理
void	prepare_pipe_parent(t_node *node, int count)
{
	// 最初のコマンド以外：前のパイプの読み取り側を閉じる
	if (count > 0)
		close(node->in_fd);
	// 最後のコマンド以外：パイプの書き込み側（使わない）を閉じる & パイプの読み取り側を次のコマンドの読み取りに
	if (node->next_cmd)
	{
		close(node->pipefd[1]);
		node->next_cmd->in_fd = node->pipefd[0];
	}
}
