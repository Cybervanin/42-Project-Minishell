/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 09:57:11 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/07 10:00:08 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_val(const char *name, t_ms *shell)
{
	int	i;
	int	len;

	if (!name || !shell || !shell->envs)
		return (NULL);
	i = 0;
	len = ft_strlen(name);
	while (shell->envs[i])
	{
		if (ft_strncmp(shell->envs[i], name, len) == 0
			&& shell->envs[i][len] == '=')
			return (shell->envs[i] + len + 1);
		i++;
	}
	return (NULL);
}

static char	*create_env_string(const char *key, char *value)
{
	char	*temp;
	char	*full;

	temp = ft_strjoin(key, "=");
	full = ft_strjoin(temp, value);
	free(temp);
	return (full);
}

static char	**reallocate_env(char *old_envs[], char *new_entry)
{
	int		i;
	char	**new_envs;

	i = 0;
	while (old_envs[i])
		i++;
	new_envs = malloc(sizeof(char *) * (i + 2));
	if (!new_envs)
		return (old_envs);
	i = 0;
	while (old_envs[i])
	{
		new_envs[i] = old_envs[i];
		i++;
	}
	new_envs[i] = new_entry;
	new_envs[i + 1] = NULL;
	free(old_envs);
	return (new_envs);
}

void	update_env_val(const char *key, char *value, t_ms *shell)
{
	int		i;
	int		len;
	char	*new_entry;

	if (!key || !value)
		return ;
	i = 0;
	len = ft_strlen(key);
	new_entry = create_env_string(key, value);
	while (shell->envs[i])
	{
		if (ft_strncmp(shell->envs[i], key, len) == 0
			&& shell->envs[i][len] == '=')
		{
			free(shell->envs[i]);
			shell->envs[i] = new_entry;
			return ;
		}
		i++;
	}
	shell->envs = reallocate_env(shell->envs, new_entry);
}

char	*remove_quotes(char *input)
{
	int		len;
	char	*no_quotes;

	len = ft_strlen(input);
	no_quotes = ft_substr(input, 1, len - 2);
	return (no_quotes);
}
