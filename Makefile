NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror

RM = rm -f

LIB_DIR = ./lib
LIB = $(LIB_DIR)/lib.a -lreadline

SRCS = ./srcs/main.c ./srcs/interpret.c ./srcs/finish_loop.c \
	./srcs/builtin/cd_move.c ./srcs/builtin/cd.c ./srcs/builtin/echo.c ./srcs/builtin/env.c ./srcs/builtin/export.c ./srcs/builtin/ft_exit.c ./srcs/builtin/inner_exit.c ./srcs/builtin/print_env.c ./srcs/builtin/pwd.c ./srcs/builtin/unset.c \
	./srcs/execution/exec_builtin.c ./srcs/execution/exec_cmd.c ./srcs/execution/execute.c ./srcs/execution/find_path.c ./srcs/execution/is_builtin.c ./srcs/execution/link_exec_nodes.c ./srcs/execution/prepare.c ./srcs/execution/wait.c \
	./srcs/tokenization/add_token.c ./srcs/tokenization/handle_metachar.c ./srcs/tokenization/handle_word.c ./srcs/tokenization/is_blank.c ./srcs/tokenization/is_metacher.c ./srcs/tokenization/is_quote.c ./srcs/tokenization/tokenize.c \
	./srcs/parsing/append_arg.c ./srcs/parsing/append_redir.c ./srcs/parsing/consume.c ./srcs/parsing/new_node.c ./srcs/parsing/parse_cmd.c ./srcs/parsing/parse_redir.c ./srcs/parsing/parse.c ./srcs/parsing/peek.c \
	./srcs/init/init.c ./srcs/init/init_utils.c \
	./srcs/expansion/exp_token.c ./srcs/expansion/expand_cmds.c ./srcs/expansion/expand_redirs.c ./srcs/expansion/expand_unsplit.c ./srcs/expansion/expand.c ./srcs/expansion/get_var_value.c ./srcs/expansion/split_exp.c ./srcs/expansion/unsplit_utils.c ./srcs/expansion/update_args.c \
	./srcs/redirection/create_temp.c ./srcs/redirection/heredoc.c ./srcs/redirection/redirect.c ./srcs/redirection/unlink_temp.c \
	./srcs/utils/event.c ./srcs/utils/free_shell.c ./srcs/utils/free.c ./srcs/utils/ft_atol.c ./srcs/utils/ft_cwd.c ./srcs/utils/ft_isspace.c ./srcs/utils/ft_malloc.c ./srcs/utils/ft_split_len.c ./srcs/utils/ft_split_longer.c ./srcs/utils/ft_strjoin_delim.c ./srcs/utils/ft_strndup.c ./srcs/utils/ft_union.c ./srcs/utils/shell.c ./srcs/utils/status.c \
	./srcs/signal/sigint.c ./srcs/signal/sigquit.c \
	./srcs/env/ft_env_utils_delete.c ./srcs/env/ft_env_utils_get.c ./srcs/env/ft_env_utils_new.c ./srcs/env/ft_env_utils_update.c ./srcs/env/ft_env.c

OBJS = $(SRCS:.c=.o)
INCLUDES = -I ./includes -I ./$(LIB_DIR)/includes

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(LIB_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIB) -o $(NAME)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) -C $(LIB_DIR) clean
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) -C $(LIB_DIR) fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
