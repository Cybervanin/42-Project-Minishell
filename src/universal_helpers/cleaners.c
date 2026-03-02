/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaners.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 15:23:20 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/02 15:23:21 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean_ms(t_ms *shell)
{
	int	i;

	i = 0;
	if (shell->envs)
	{
		while (shell->envs[i])
		{
			free(shell->envs[i]);
			i++;
		}
		free(shell->envs);
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
