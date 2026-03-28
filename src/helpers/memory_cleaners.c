/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_cleaners.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victde-s <victde-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:23:20 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/28 20:11:36 by victde-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*cmd;
	t_cmd	*temp;
	t_redir	*temp_redir;

	cmd = cmd_list;
	while (cmd)
	{
		temp = cmd->next;
		free_matrix(cmd->args);
		while (cmd->redirs)
		{
			temp_redir = cmd->redirs->next;
			if (cmd->redirs->heredoc_fd >= 0)
				close(cmd->redirs->heredoc_fd);
			free(cmd->redirs->target);
			free(cmd->redirs);
			cmd->redirs = temp_redir;
		}
		free(cmd);
		cmd = temp;
	}
}

void	free_token_list(t_token *token_list)
{
	int	i;

	i = 0;
	if (token_list)
	{
		while (token_list[i].value)
			free(token_list[i++].value);
		free(token_list);
	}
}

void	free_matrix(char **m)
{
	int	i;

	i = 0;
	while (m && m[i])
		free(m[i++]);
	free(m);
}
