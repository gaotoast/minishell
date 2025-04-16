/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yumiyao <yumiyao@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 01:29:37 by yumiyao           #+#    #+#             */
/*   Updated: 2025/04/16 23:36:24 by yumiyao          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_string(char *bufs)
{
	size_t	size;
	char	*rtn;

	size = 512;
	rtn = NULL;
	// TODO 校舎のLinux上でPATH_MAXが使えるか確認
	// 使えたらPATH_MAXにする
	// 使えなかったら4096(PATH_MAXとして一般的な値に決め打ち)
	while (size <= 32)
	{
		bufs = malloc(size);
		if (!bufs)
			return (NULL);
		rtn = getcwd(bufs, size);
		if (rtn)
			return (rtn);
		free(bufs);
		if (errno != ERANGE)
			return (NULL);
		size *= 2;
	}
	return (NULL);
}

int	pwd(int argc, char **argv)
{
	char	*bufs;

	if (argc > 1 && argv)
	{
		// TODO エラー出力
		// bad option, or too many arguments
		// -X (xは文字)だった場合bad option, それ以外の場合 too many arguments
		// bad option の際にargvを使用するためエラー処理を実装すればこのif文の
		// "&& argv" は消してしまってよい
	}
	bufs = get_string(bufs);
	// TODO エラー処理
	// malloc以外でのエラーでnullが返ってきたときどうする？
	if (!bufs)
		return (1);
	printf("%s\n", bufs);
	free(bufs);
	return (0);
}
