/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: victde-s <victde-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 17:44:22 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/23 19:03:50 by victde-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	builtin_env(t_ms *shell)
{
	int	i;

	i = 0;
	if (!shell || !shell->envs)
	{
		shell->last_status = 1;
		return ;
	}
	while (shell->envs[i])
	{
		write(1, shell->envs[i], ft_strlen(shell->envs[i]));
		write(1, "\n", 1);
		i++;
	}
	shell->last_status = 0;
}

static void	create_new_envs(t_ms *shell, char *name, char	***new_envs)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (shell->envs[i])
	{
		if (ft_strncmp(shell->envs[i], name, ft_strlen(name)) == 0
			&& shell->envs[i][ft_strlen(name)] == '=')
		{
			free(shell->envs[i]);
			i++;
			continue ;
		}
		(*new_envs)[j] = shell->envs[i];
		i++;
		j++;
	}
}

void	builtin_unset(t_ms *shell, char *name)
{
	int		max;
	char	**new_envs;

	max = 0;
	if (!name || !shell->envs || !does_env_exist(shell->envs, name))
		return ;
	while (shell->envs[max])
		max++;
	new_envs = ft_calloc(max, sizeof(char *));
	if (!new_envs)
		return ;
	create_new_envs(shell, name, &new_envs);
	free(shell->envs);
	shell->envs = new_envs;
	shell->last_status = 0;
}
