/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 00:47:01 by yumiyao           #+#    #+#             */
/*   Updated: 2025/04/17 01:44:09 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	env(int argc, char **argv, char **envp)
{
	int	i;

	i = 0;
	if (argc > 1 && argv && !envp)
	{
		// TODO エラー出力
		// bad option
		// -X (xは文字)だった場合bad option
		// bad option の際にargvを使用するためエラー処理を実装すればこのif文の
		// "&& argv" は消してしまってよい
	}
	while (envp[i])
	{
		printf("%s\n", envp[i++]);
	}
	return (0);
}
