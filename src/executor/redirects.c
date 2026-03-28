/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victde-s <victde-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 17:41:11 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/28 20:11:36 by victde-s         ###   ########.fr       */
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
	if (redir->type == HEREDOC)
	{
		if (redir->heredoc_fd < 0)
			return (-1);
		if (dup2(redir->heredoc_fd, STDIN_FILENO) < 0)
			return (perror("dup2"), -1);
		close(redir->heredoc_fd);
		redir->heredoc_fd = -1;
	}
	(void)shell;
	return (0);
}

int	apply_redirects(t_ms *shell)
{
	t_redir	*redir;

	redir = shell->cmd_list->redirs;
	while (redir)
	{
		if (apply_single_redirect(redir, shell) < 0)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
