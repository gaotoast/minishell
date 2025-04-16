#ifndef MINISHELL_H
# define MINISHELL_H

# include "ft_printf.h"
# include "get_next_line.h"
# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// execution
// child.c
void	exec_if_relative_path(char **cmds, char **envp);
void	exec_if_absolute_path(char **cmds, char **envp);
void	execute_in_child(char **cmds, char **envp);

// execute.c
int		execute(char **cmds, char **envp);

// find_path.c
char	*search_path(char *cmd_name, char **path_list);
char	*resolve_cmd_path(char *cmd, char *path_env);

// free.c
void	free_2d_array(void **array);

#endif