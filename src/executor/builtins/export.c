/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-cas <jode-cas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 17:44:02 by jode-cas          #+#    #+#             */
/*   Updated: 2026/03/28 13:42:02 by jode-cas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_valid_identifier(char *name)
{
	int	i;

	if (!name || (!ft_isalpha((unsigned char)name[0]) && name[0] != '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum((unsigned char)name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	sort_envs_alphabetically(char **envs)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (envs[i])
	{
		j = 0;
		while (envs[j + 1])
		{
			if (ft_strncmp(envs[j], envs[j + 1], ft_strlen(envs[j]) + 1) > 0)
			{
				temp = envs[j];
				envs[j] = envs[j + 1];
				envs[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_declared(t_ms *shell)
{
	char	*eq;
	int		i;

	i = 0;
	sort_envs_alphabetically(shell->envs);
	while (shell->envs[i])
	{
		ft_putstr_fd("declare -x ", 1);
		eq = ft_strchr(shell->envs[i], '=');
		if (!eq)
		{
			ft_putstr_fd(shell->envs[i], 1);
			ft_putchar_fd('\n', 1);
			return ;
		}
		write(1, shell->envs[i], eq - shell->envs[i]);
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(eq + 1, 1);
		ft_putstr_fd("\"\n", 1);
		i++;
	}
	shell->last_status = 0;
}

static int	process_arg(char *arg, t_ms *shell)
{
	char	*eq;
	char	*key;

	eq = ft_strchr(arg, '=');
	if (eq)
		key = ft_substr(arg, 0, eq - arg);
	else
		key = ft_strdup(arg);
	if (!key)
		return (1);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: '", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		free(key);
		return (1);
	}
	if (eq)
		update_env_val(key, eq + 1, shell);
	free(key);
	return (0);
}

void	builtin_export(char **args, t_ms *shell)
{
	int	i;
	int	status;

	if (!args[1])
	{
		print_declared(shell);
		return ;
	}
	i = 1;
	status = 0;
	while (args[i])
	{
		if (process_arg(args[i], shell) == 1)
			status = 1;
		i++;
	}
	shell->last_status = status;
}
