/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 17:44:22 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/02 17:48:11 by jode-cas         ###   ########.fr       */
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

static char	does_env_exist(char **envs, char *name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (envs[i])
	{
		if (ft_strncmp(envs[i], name, len) == 0 && envs[i][len] == '=')
			return (1);
		i++;
	}
	return (0);
}

static void	create_new_envs(t_ms *shell, int max, char *name, char	***new_envs)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (shell->envs[i])
	{
		if (ft_strncmp(shell->envs[i], name, ft_strlen(name)) == 0
			&& shell->envs[i][ft_strlen(name)] == '=')
			i++;
		if (i == max)
			break ;
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
	create_new_envs(shell, max, name, &new_envs);
	free(shell->envs);
	shell->envs = new_envs;
	shell->last_status = 0;
}
