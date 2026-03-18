/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 09:55:52 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/07 09:55:53 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	is_delimiter_quotted(char *delimiter)
{
	int	delim_size;

	delim_size = ft_strlen(delimiter);
	if (delimiter[0] == '"' && delimiter[delim_size - 1] == '"')
		return (1);
	if (delimiter[0] == '\'' && delimiter[delim_size - 1] == '\'')
		return (1);
	return (0);
}

static void	expand_line(char **line, t_ms *shell)
{
	int	i;

	i = 0;
	while ((*line)[i])
	{
		if ((*line)[i] == '$')
			rebuild_string(line, i, shell);
		i++;
	}
}

static void	read_heredoc(int *fd, char *delimiter, t_ms *shell)
{
	char	*line;

	set_signals_child();
	close(fd[0]);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (!is_delimiter_quotted(delimiter))
			expand_line(&line, shell);
		if (heredoc_stop_condition(line, delimiter))
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	exit(0);
}

static int	handle_heredoc(char *delimiter, t_ms *shell)
{
	int	fd[2];
	pid_t heredoc_pid;
	int status;

	if (pipe(fd) < 0)
		return (-1);
	heredoc_pid = fork();
	if (heredoc_pid == 0)
		read_heredoc(fd, delimiter, shell);
	set_signals_exec();
	while (waitpid(heredoc_pid, &status, 0) == -1) {
    if (errno != EINTR)
			break;
	}
	set_signals();
	close(fd[1]);
	return (fd[0]);
}

int	redirect_heredoc(char *delimiter, t_ms *shell)
{
	int	fd_in;

	fd_in = handle_heredoc(delimiter, shell);
	if (fd_in < 0)
		return (-1);
	dup2(fd_in, STDIN_FILENO);
	close(fd_in);
	return (0);
}
