/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victde-s <victde-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 17:41:11 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/28 17:47:57 by victde-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	redirect_output(char *target, int flags)
{
	int	file;

	file = open(target, flags, 0644);
	if (file < 0)
	{
		perror("open");
		return (-1);
	}
	dup2(file, STDOUT_FILENO);
	close(file);
	return (0);
}

static int	redirect_input(char *target)
{
	int	file;

	file = open(target, O_RDONLY);
	if (file < 0)
	{
		perror("open");
		return (-1);
	}
	dup2(file, STDIN_FILENO);
	close(file);
	return (0);
}

static int	apply_single_redirect(t_redir *redir, t_ms *shell)
{
	if (redir->type == REDIRECT_OUT)
		return (redirect_output(redir->target, O_CREAT | O_WRONLY | O_TRUNC));
	if (redir->type == REDIRECT_APPEND)
		return (redirect_output(redir->target, O_CREAT | O_WRONLY | O_APPEND));
	if (redir->type == REDIRECT_IN)
		return (redirect_input(redir->target));
	(void)shell;
	return (0);
}

static int	apply_heredoc_redirect(t_redir *redir, t_ms *shell, int *last_fd)
{
	int	heredoc_fd;

	heredoc_fd = redirect_heredoc(redir->target, shell);
	if (heredoc_fd < 0)
		return (-1);
	if (*last_fd >= 0)
		close(*last_fd);
	*last_fd = heredoc_fd;
	return (0);
}

int	apply_redirects(t_ms *shell)
{
	t_redir	*redir;
	int		last_fd;

	last_fd = -1;
	redir = shell->cmd_list->redirs;
	while (redir)
	{
		if (redir->type == HEREDOC && apply_heredoc_redirect
			(redir, shell, &last_fd) < 0)
			return (close(last_fd), -1);
		else
		{
			if (apply_single_redirect(redir, shell) < 0)
				return (close(last_fd), -1);
			if (redir->type == REDIRECT_IN && last_fd >= 0)
				last_fd = (close(last_fd), -1);
		}
		redir = redir->next;
	}
	if (last_fd >= 0)
		dup2(last_fd, STDIN_FILENO);
	if (last_fd >= 0)
		close(last_fd);
	return (0);
}
