/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:13:10 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/28 16:47:37 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libs/libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	HEREDOC,
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
}					t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*target;
	struct s_redir	*next;
}					t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_minishell
{
	char			**envs;
	int				last_status;
	struct s_cmd	*cmd_list;
	int				initial_stdout;
	int				initial_stdin;
}					t_ms;

t_token				*lexit(char *input);
int					array_length(char **array);
t_token_type		get_token_type(char *word);
char				**word_splitter(char const *s);
char				is_whitechar(char c);
char				is_metachar(char c);
void				skip_until_next_quote(char *s, int *i);
char				boolean_invert(char value);
t_cmd				*parser(t_token *token_list);
char				is_builtin(char *input);
t_ms				*create_shell_instance(char **envs);
void				expander(t_ms *shell);
char				*get_env_val(const char *name, t_ms *shell);
void				update_env_val(const char *key, char *value, t_ms *shell);
char				*remove_quotes(char *input);
void				executor(t_ms *shell);
void				free_matrix(char **m);
void				builtin_echo(t_ms *shell);
void				builtin_env(t_ms *shell);
void				builtin_cd(char **args, t_ms *shell);
void				builtin_export(char **args, t_ms *shell);
void				builtin_unset(t_ms *shell, char *name);
void				builtin_exit(t_ms *shell);
int					apply_redirects(t_ms *shell);
int					redirect_heredoc(char *delimiter, t_ms *shell);
void				rebuild_string(char **og_str, int i, t_ms *shell);
char				is_delimiter_quotted(char *delimiter);
char				heredoc_stop_condition(char *line, char *delimiter);
void				call_path(t_ms *shell, char *cmd);
char				**get_path_dirs(t_ms *shell);
char				*get_full_cmd_path(t_ms *shell);
void				call_builtins(t_ms *shell);
void				sigint_handler(int sig);
void				set_signals(void);
void				set_signals_child(void);
void				set_signals_exec(void);
void				await_results(t_ms *shell, pid_t child_pid);
char				is_empty_command(char *input);
char				does_env_exist(char **envs, char *name);
void				handle_sigint_last_status(t_ms *shell);
void				free_token_list(t_token *token_list);
void				free_cmd_list(t_cmd *cmd_list);
void				display_error(char *str, const char *error_msg);
char				is_dir(const char *path, t_ms *shell);
char				**deep_copy(char **matrix);
void				set_signals_heredoc(void);
void				heredoc_loop(int *fd, char *delimiter, t_ms *shell);

extern int			g_sigint;

#endif