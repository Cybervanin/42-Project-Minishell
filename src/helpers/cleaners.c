/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaners.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:23:20 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/17 19:57:30 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_minishell_memory(t_ms *shell, t_token	*token_list)
{
	int i;

	i = 0;
	if (shell->cmd_list)
	{
		free_matrix(shell->cmd_list->args);
		if (shell->cmd_list->redirs)
			while (shell->cmd_list->redirs[i].target)
			{
				free(shell->cmd_list->redirs[i].target);
				free(shell->cmd_list->redirs[i++].next);
			}
		free(shell->cmd_list->redirs);
		free(shell->cmd_list);
	}
	if (shell->envs)
		free_matrix(shell->envs);
	free(shell);
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
