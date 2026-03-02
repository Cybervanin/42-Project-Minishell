/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:15:13 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/02 15:15:24 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	store_envs(t_ms *shell, char **envs)
{
	int	i;

	i = 0;
	while (envs[i])
		i++;
	shell->envs = ft_calloc(sizeof(char *), i + 1);
	if (!shell->envs)
		return ;
	i = 0;
	while (envs[i])
	{
		shell->envs[i] = ft_strdup(envs[i]);
		i++;
	}
}

t_ms	*create_shell_instance(char **envs)
{
	t_ms	*shell;

	shell = ft_calloc(sizeof(t_ms *), 2);
	store_envs(shell, envs);
	shell->last_status = 0;
	return (shell);
}
