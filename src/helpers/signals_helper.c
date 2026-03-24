/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 19:45:13 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/24 19:45:13 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	set_wait_status(t_ms *shell, int status)
{
	if (WIFEXITED(status))
		shell->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		shell->last_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		else if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
}

static void	wait_other_children(t_cmd *current)
{
	while (current->next)
	{
		while (wait(NULL) == -1)
		{
			if (errno != EINTR)
				break ;
		}
		current = current->next;
	}
}

void	await_results(t_ms *shell, pid_t child_pid)
{
	int		status;
	t_cmd	*current;

	current = shell->cmd_list;
	wait_other_children(current);
	while (waitpid(child_pid, &status, 0) == -1)
	{
		if (errno != EINTR)
			return ;
	}
	set_wait_status(shell, status);
}
