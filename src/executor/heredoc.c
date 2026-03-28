/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 09:55:52 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/28 16:51:20 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	read_heredoc(int *fd, char *delimiter, t_ms *shell)
{
	set_signals_heredoc();
	close(fd[0]);
	heredoc_loop(fd, delimiter, shell);
	close(fd[1]);
	exit(0);
}

static char	get_fork_return(pid_t heredoc_pid, t_ms *shell, int *fd)
{
	int	status;

	status = 0;
	while (waitpid(heredoc_pid, &status, 0) == -1)
	{
		if (errno != EINTR)
			break ;
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		g_sigint = 1;
	if (WIFSIGNALED(status))
	{
		shell->last_status = 128 + WTERMSIG(status);
		close(fd[0]);
		return (-1);
	}
	return (0);
}

static int	handle_heredoc(char *delimiter, t_ms *shell)
{
	pid_t	heredoc_pid;
	int		fd[2];

	if (pipe(fd) < 0)
		return (-1);
	heredoc_pid = fork();
	if (heredoc_pid < 0)
	{
		perror("fork at heredoc");
		close(fd[0]);
		close(fd[1]);
		return (-1);
	}
	if (heredoc_pid == 0)
		read_heredoc(fd, delimiter, shell);
	set_signals_exec();
	if (get_fork_return(heredoc_pid, shell, fd) < 0)
	{
		close(fd[1]);
		set_signals();
		return (-1);
	}
	close(fd[1]);
	set_signals();
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
