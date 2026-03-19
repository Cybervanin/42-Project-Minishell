NAME			= minishell

LIBFT_DIR		= ./libs/libft
LIBFT			= $(LIBFT_DIR)/libft.a	
LDLFLAGS		= -L$(LIBFT_DIR) -lft -lreadline

CC 				= cc
CFLAGS 			= -Wall -Werror -Wextra -I.

SRCS 			= src/minishell.c \
						src/lexer/lexer.c \
						src/lexer/utils.c \
						src/lexer/splitter.c \
						src/helpers/universal_helpers.c \
						src/helpers/universal_helpers_2.c \
						src/helpers/env_helpers.c \
						src/helpers/exec_helpers.c \
						src/helpers/shell_helpers.c \
						src/helpers/cleaners.c \
						src/parser/parser.c \
						src/parser/expander.c \
						src/executor/executor.c \
						src/executor/redirects.c \
						src/executor/heredoc.c \
						src/executor/builtins/echo.c \
						src/executor/builtins/env.c \
						src/executor/builtins/cd.c \
						src/executor/builtins/export.c \
						src/executor/builtins/exit.c \
						src/signals/signals.c
OBJS 			= $(SRCS:%.c=%.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LDLFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re